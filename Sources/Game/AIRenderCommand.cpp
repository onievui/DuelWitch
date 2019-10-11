#include "AIRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// AI描画コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void AIRenderCommand::Initialize(Player& player) {
	player;
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

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
}

/// <summary>
/// AI描画コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void AIRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	player, timer;
}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void AIRenderCommand::Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	// ダメージ後はプレイヤーを点滅させる
	const auto& ref_status = GetStatus(player);
	if (ref_status.damageTimer <= 0.0f || sin(ref_status.damageTimer*Math::PI2 * 2) > 0) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::BloomModel)->GetResource();
		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		const DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
		// モデルを描画する
		model->Draw(context, *states, GetTransform(player).GetMatrix(), view, proj);
		// 当たり判定を描画する
		GetCollider(player).Render(view, proj, DirectX::SimpleMath::Color(1, 1, 1, 0.3f), true);
	}

	// HPバーを描画する
	RenderHpBar(player, spriteBatch);
}

/// <summary>
/// HPバーを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void AIRenderCommand::RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::HpBar);

	const Camera& camera = GetCamera(player);
	const Transform& transform = GetTransform(player);
	const float scale = 0.15f;

	// 視線ベクトルとのなす角を求める
	DirectX::SimpleMath::Vector3 eye = camera.GetEyeVector();
	DirectX::SimpleMath::Vector3 target = transform.GetPosition() - camera.GetEyePosition();
	// カメラより後ろにあるなら処理しない
	if (eye.Dot(target) < 0.0f) {
		return;
	}

	// HPバーのワールド座標
	DirectX::SimpleMath::Vector3 world_pos = transform.GetPosition() + DirectX::SimpleMath::Vector3(0, 3, 0);
	// HPバーのスクリーン座標
	DirectX::SimpleMath::Vector2 screen_pos = camera.WorldToScreenPoint(world_pos);

	// 黒の部分を描画する
	spriteBatch->Draw(texture->GetResource(2).Get(), screen_pos, nullptr,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);

	// 画像の矩形を作成する
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const Player::Status& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// HPの割合に応じて描画範囲を決める
	rect.right = static_cast<LONG>(size.x*status.hp / status.maxHp);
	rect.bottom = static_cast<LONG>(size.y);

	// 赤の部分を描画する
	spriteBatch->Draw(texture->GetResource(1).Get(), screen_pos, &rect,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);
	// 緑の部分を描画する
	spriteBatch->Draw(texture->GetResource(0).Get(), screen_pos, &rect,
		DirectX::Colors::White, 0, texture->GetCenter(), scale);
}
