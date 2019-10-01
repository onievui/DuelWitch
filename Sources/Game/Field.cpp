#include "Field.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>


Field::Field() {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// コモンステートを作成する
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// エフェクトファクトリーを作成する
	std::unique_ptr<DirectX::EffectFactory> fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// CMOを読み込んでスカイドームを作成する
	m_skydome = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Protected/skydome.cmo", *fxFactory);

	m_skydome->UpdateEffects([](DirectX::IEffect* effect) {
		//DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		//if (lights) {
		//	lights->SetLightingEnabled(true);
		//	lights->SetPerPixelLighting(true);
		//	lights->SetAmbientLightColor(DirectX::Colors::White);
		//}

		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights)
		{
			// ライトの影響をなくす 
			lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
			lights->SetLightEnabled(0, false);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
		DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basicEffect)
		{
			// エミッション色を白に設定する 
			basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
		}
	});

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Protected/u_turn.png", nullptr, m_turnTexture.GetAddressOf());

	// 壁の生成
	m_wall = DirectX::GeometricPrimitive::CreateSphere(context, 160.0f, 8U, false, true);
}

Field::~Field() {
}

void Field::Update() {
}

void Field::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(1.0f);
	m_skydome->Draw(context, *m_states, world, view, proj);

	world = DirectX::SimpleMath::Matrix::CreateScale(4.0f);
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(Math::PI);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 3, -82);
	DrawTurn(world, view, proj);

	world = DirectX::SimpleMath::Matrix::CreateScale(4.0f);
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(0);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 3, 82);
	DrawTurn(world, view, proj);
	world = DirectX::SimpleMath::Matrix::CreateScale(1.0f);
	m_wall->Draw(world, view, proj, DirectX::SimpleMath::Color(30/255.0f, 130/255.0f, 240/255.0f, 1.0f), nullptr, true);
}

void Field::DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// 頂点情報
	DirectX::VertexPositionTexture vertex[4] =
	{
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f),   DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(-0.5f, 0.5f, 0.0f),  DirectX::SimpleMath::Vector2(1.0f, 0.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(-0.5f, -0.5f, 0.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(0.5f,-0.5f, 0.0f),   DirectX::SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	device->CreateBlendState(&bddesc, &blendstate);

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// カリングは左周り
	context->RSSetState(m_states->CullNone());
	// 不透明のみ描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetReferenceAlpha(0);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(view);
	m_batchEffect->SetProjection(proj);
	m_batchEffect->SetTexture(m_turnTexture.Get());
	//m_batchEffect->SetAlpha(1.0f);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// 半透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

