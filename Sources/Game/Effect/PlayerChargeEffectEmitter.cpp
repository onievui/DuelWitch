#include "PlayerChargeEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ResourceManager.h>
#include <Utils\ConstBuffer.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera\Camera.h>
#include "PlayerChargeEffect.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerChargeEffectEmitter::PlayerChargeEffectEmitter() {
	// メモリを確保しておく
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam.particleNum);
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<PlayerChargeEffect>();
	}

	// 定数バッファの作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BillboardBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());

}

/// <summary>
/// エフェクトを生成する
/// </summary>
/// <param name="pos"></param>
/// <param name="dir"></param>
void PlayerChargeEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	
	m_chargeState = State::None;
	m_transform.SetPosition(pos);
	
	float delay_interval = parameter.lifeTime / m_effects.size();
	float interval = 0.0f;

	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Initialize(parameter.lifeTime, parameter.appearPosOffset, dir, dir);
		(*itr)->SetDelayTime(interval);
		interval += delay_interval;
	}
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
/// <param name="camera">カメラ</param>
void PlayerChargeEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	// 視線ベクトルを取得する
	m_eyeVec = camera->GetEyeVector();

	// エフェクトを更新する
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Update(timer);
	}
}

/// <summary>
/// エフェクトを描画する
/// </summary>
/// <param name="batch">プリミティブバッチ</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void PlayerChargeEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	const EffectParameter::player_charge_param parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// チャージ状態でないなら描画しない
	if (m_chargeState == State::None) {
		return;
	}

	// 定数バッファの作成
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_transform.GetMatrix().Transpose();
	cbuff.eye = DirectX::SimpleMath::Vector4(m_eyeVec.x, m_eyeVec.y, m_eyeVec.z, 0);
	cbuff.lookAt = false;
	cbuff.diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//定数バッファの内容更新
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// 透明判定処理
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// 深度バッファに参照する
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// 時計回りカリング
	context->RSSetState(states->CullClockwise());

	// シェーダに定数バッファを割り当てる
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->GSSetConstantBuffers(0, 1, cb);

	// ピクセルシェーダにサンプラーを割り当てる
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	// 各シェーダを割り当てる
	VertexShaderResource* vertex_shader = ServiceLocater<ResourceManager<VertexShaderResource>>::Get()
		->GetRawResource(VertexShaderID::Default);
	const GeometryShaderResource* geometry_shader = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get()
		->GetResource(GeometryShaderID::Billboard);
	const PixelShaderResource* pixel_shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()
		->GetResource(PixelShaderID::Default);
	context->VSSetShader(vertex_shader->GetResource().Get(), nullptr, 0);
	context->GSSetShader(geometry_shader->GetResource().Get(), nullptr, 0);
	context->PSSetShader(pixel_shader->GetResource().Get(), nullptr, 0);

	// 入力レイアウトを割り当てる
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// 頂点情報を作成する
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	vertex.reserve(m_effects.size());
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		// 開始していないエフェクトは飛ばす
		if ((*itr)->IsWaiting()) {
			continue;
		}
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), GetColor(),
			DirectX::SimpleMath::Vector2((*itr)->GetScale(), 0.0f) // xがスケール yがZ回転
		));
	}

	// テクスチャを割り当てる
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::PlayerCharge);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// エフェクトを描画する
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}

/// <summary>
/// 親オブジェクトを設定する
/// </summary>
/// <param name="pParent">親オブジェクトへのポインタ</param>
void PlayerChargeEffectEmitter::SetParent(const Transform* pParent) {
	m_pParent = pParent;
	m_transform.SetParent(pParent);
}

/// <summary>
/// 色を取得する
/// </summary>
/// <returns>
/// エフェクトの色
/// </returns>
DirectX::SimpleMath::Color PlayerChargeEffectEmitter::GetColor() const {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	DirectX::SimpleMath::Color color;
	// チャージ状態によって色を分ける
	switch (m_chargeState) {
	case PlayerChargeEffectEmitter::State::Charge0:
		color = parameter.colorCharge0;
		break;
	case PlayerChargeEffectEmitter::State::Charge1:
		color = parameter.colorCharge1;
		break;
	case PlayerChargeEffectEmitter::State::Charge2:
		color = parameter.colorCharge2;
		break;
	default:
		ErrorMessage(L"プレイヤーのチャージエフェクトの状態が不正です");
		break;
	}

	return color / 255.0f;
}