#include "FieldShieldEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ResourceManager.h>
#include <Utils\ConstBuffer.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "FieldShieldEffect.h"
#include "Camera.h"


/// <summary>
/// コンストラクタ
/// </summary>
FieldShieldEffectEmitter::FieldShieldEffectEmitter() {
	// メモリを確保しておく
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam.particleNum);
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<FieldShieldEffect>();
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
/// エフェクトエミッターを生成する
/// </summary>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
void FieldShieldEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// AddEffcet関数からエフェクトを追加する
	pos; dir;
}

/// <summary>
/// エフェクトエミッターを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
/// <param name="camera">カメラ</param>
void FieldShieldEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	camera;

	// エフェクトを更新する
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		if ((*itr)->IsUsed()) {
			(*itr)->Update(timer);
		}
	}
}

/// <summary>
/// エフェクトエミッターを描画する
/// </summary>
/// <param name="batch">プリミティブバッチ</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FieldShieldEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// 頂点情報を作成する
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		if (!(*itr)->IsUsed()) {
			continue;
		}
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), (*itr)->GetColor(),
			DirectX::SimpleMath::Vector2((*itr)->GetScale(), 0) // xがスケール yがZ回転
		));
	}

	// エフェクトが出現していない場合は処理しない
	if (vertex.size() == 0) {
		return;
	}

	// 定数バッファの作成
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
	const DirectX::SimpleMath::Vector3& eye_pos = m_pParent->GetLocalPosition();
	cbuff.eye = DirectX::SimpleMath::Vector4(eye_pos.x, eye_pos.y, eye_pos.z, 0);
	cbuff.lookAt = true;
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//定数バッファの内容更新
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = states->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
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
	// テクスチャを割り当てる
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::FieldShield);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// 入力レイアウトを割り当てる
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// エフェクトを描画する
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}

/// <summary>
/// エフェクトを追加する
/// </summary>
/// <param name="pos">座標</param>
/// <param name="normal">方向</param>
void FieldShieldEffectEmitter::AddEffect(const DirectX::SimpleMath::Vector3& pos) {
	// 未使用のエフェクトを探す
	std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr =
		LamdaUtils::FindIfNot(m_effects, LamdaUtils::GetLamda(&FieldShieldEffect::IsUsed));

	if (itr == m_effects.end()) {
		return;
	}

	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;

	// 初期化して追加
	(*itr)->Initialize(parameter.lifeTime, pos);
}
