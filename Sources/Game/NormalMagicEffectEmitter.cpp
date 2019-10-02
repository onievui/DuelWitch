#include "NormalMagicEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ConstBuffer.h>
#include <Utils\ResourceManager.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "Camera.h"
#include "NormalMagicEffect.h"


/// <summary>
/// コンストラクタ
/// </summary>
NormalMagicEffectEmitter::NormalMagicEffectEmitter() {
	// メモリを確保しておく
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->normalMagicParam.particleNum);
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<NormalMagicEffect>();
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
/// <param name="pos">出現位置</param>
/// <param name="dir">向き</param>
void NormalMagicEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	dir;
	m_transform.SetPosition(pos);

	// エフェクトの初期化
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Initialize();
	}
}

/// <summary>
/// エフェクトエミッターを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
/// <param name="camera">カメラ</param>
void NormalMagicEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	// 視線ベクトルを取得する
	m_eyeVec = camera->GetEyeVector();

	// エフェクトを更新する
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Update(timer);
	}
}

/// <summary>
/// エフェクトエミッターを描画する
/// </summary>
/// <param name="batch">プリミティブバッチ</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void NormalMagicEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// 定数バッファの作成
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_pParent->GetMatrix().Transpose();
	cbuff.eye = DirectX::SimpleMath::Vector4(m_eyeVec.x, m_eyeVec.y, m_eyeVec.z, 0);
	cbuff.lookAt = false;
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
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Particle);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// 入力レイアウトを割り当てる
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// 頂点情報を作成する
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	const float partice_scale = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->normalMagicParam.scale;
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), DirectX::Colors::White,
			DirectX::SimpleMath::Vector2(partice_scale, 0) // xがスケール yがZ回転
		));
	}
	
	// エフェクトを描画する
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}
