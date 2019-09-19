#include "UserRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include "TargetCamera.h"


/// <summary>
/// ユーザ描画コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void UserRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	timer;
	// 相手プレイヤーが画面外にいる場合、方向を表示するためのUIの座標を計算する
	const Camera& camera = GetCamera(player);
	const TargetCamera* target_camera = dynamic_cast<const TargetCamera*>(&camera);
	// 画角を取得して、画面外かどうかの範囲を決める
	float area_angle;
	if (target_camera) {
		area_angle = target_camera->GetFov();
	}
	else {
		area_angle = Math::HarfPI;
	}
	const DirectX::SimpleMath::Vector3& camera_pos = camera.GetEyePosition();
	const DirectX::SimpleMath::Vector3& enemy_pos = GetTransform(GetOtherPlayer(player)).GetPosition();
	DirectX::SimpleMath::Vector3 camera_dir = camera.GetCameraVector();
	DirectX::SimpleMath::Vector3 other_dir = enemy_pos - camera_pos;
	float angle = std::acosf(camera_dir.Dot(other_dir) / (camera_dir.Length()*other_dir.Length()));
	// カメラの向きと敵の方向が一定の角度以内であれば処理しない
	if (angle < area_angle) {
		// アイコンの描画を無効にする
		m_enableRenderTargetIcon = false;
		return;
	}
	// 敵プレイヤーへのベクトル
	DirectX::SimpleMath::Vector3 vec;
	// カメラの方向を-Z方向に向ける回転行列を生成する
	if (camera_dir.z <= 0.0f) {
		DirectX::SimpleMath::Quaternion rotation = Math::CreateQuaternionFromVector3(camera_dir, -DirectX::SimpleMath::Vector3::UnitZ);
		// 敵プレイヤーへのベクトルを既定の方向の回転させる
		vec = DirectX::SimpleMath::Vector3::Transform(other_dir, rotation);
	}
	else {
		DirectX::SimpleMath::Quaternion rotation = Math::CreateQuaternionFromVector3(camera_dir, DirectX::SimpleMath::Vector3::UnitZ);
		// 敵プレイヤーへのベクトルを既定の方向の回転させる
		vec = DirectX::SimpleMath::Vector3::Transform(other_dir, rotation);
		// Y軸で180度回転させる
		vec *= DirectX::SimpleMath::Vector3(-1, 1, -1);
		

	}
	constexpr float screen_offset = 50.0f;
	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));
	// アイコンの座標を計算する
	m_targetIconPos = CalculateIconPos(vec, screen_size, DirectX::SimpleMath::Vector2(screen_offset,screen_offset));
	// アイコンの描画を有効にする
	m_enableRenderTargetIcon = true;

}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::Render(const Player& player, const DirectX::SimpleMath::Matrix& view,
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
		GetCollider(player).Render(view, proj);
	}

	// 相手プレイヤーが画面外にいる場合、アイコンを表示する
	if (m_enableRenderTargetIcon) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
		spriteBatch->Draw(texture->GetResource().Get(),
			m_targetIconPos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
			texture->GetCenter(), DirectX::SimpleMath::Vector2(0.2f, 0.2f));
	}

	// 所持エレメントを描画する
	const std::list<ElementID>& have_elements = GetHaveElements(player);
	int i = have_elements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = have_elements.rbegin(); itr != have_elements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 630.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.5f, 1.5f));
		--i;
	}

	// 照準を描画する
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	const DirectX::SimpleMath::Vector2& mouse_pos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	spriteBatch->Draw(texture->GetResource().Get(),
		mouse_pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}

/// <summary>
/// ターゲットの方向を示すアイコンの位置を計算する
/// </summary>
/// <param name="vec">ターゲットの方向</param>
/// <param name="screenSize">画面サイズ</param>
/// <param name="screenOffset">アイコンの描画位置の制限</param>
/// <returns>
/// ターゲットの方向を示すアイコンの位置
/// </returns>
DirectX::SimpleMath::Vector2 UserRenderCommand::CalculateIconPos(const DirectX::SimpleMath::Vector3& vec,
	const DirectX::SimpleMath::Vector2& screenSize, const DirectX::SimpleMath::Vector2& screenOffset) {
	// ターゲットの方向を示すアイコンの位置
	DirectX::SimpleMath::Vector2 icon_pos;

	// 平行の場合
	if (vec.y == 0.0f) {
		icon_pos.y = screenSize.y * 0.5f;
		// 左右の確認
		icon_pos.x = (vec.x <= 0.0f ? screenOffset.x : screenSize.x - screenOffset.x);
		return icon_pos;
	}
	// 垂直の場合
	if (vec.x == 0.0f) {
		icon_pos.x = screenSize.x * 0.5f;
		// 上下の確認
		icon_pos.y = (vec.y >= 0.0f ? screenOffset.y : screenSize.y - screenOffset.y);
		return icon_pos;
	}
	
	// 斜めの場合
	// オフセットを加えたスクリーンの半分のサイズ
	DirectX::SimpleMath::Vector2 harf_screen_size = screenSize * 0.5f - screenOffset;
	// スクリーンの縦横比
	float screen_aspect = harf_screen_size.x / harf_screen_size.y;
	// ターゲットの方向の縦横比
	float target_aspect = std::fabsf(vec.x / vec.y);
	
	// 左右方向の場合
	if (target_aspect >= screen_aspect) {
		// 左右の確認
		icon_pos.x = (vec.x <= 0.0f ? screenOffset.x : screenSize.x - screenOffset.x);
		// Y座標の計算
		icon_pos.y = screenSize.y*0.5f - (harf_screen_size.x / std::fabsf(vec.x))*vec.y;
	}
	// 上下方向の場合
	else {
		// 上下の確認
		icon_pos.y = (vec.y >= 0.0f ? screenOffset.y : screenSize.y - screenOffset.y);
		// X座標の計算
		icon_pos.x = screenSize.x*0.5f + (harf_screen_size.y / std::fabsf(vec.y))*vec.x;
	}
	return icon_pos;
}
