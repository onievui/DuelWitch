#include "UserRenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\MouseWrapper.h>
#include <Game\Camera\TargetCamera.h>


/// <summary>
/// ユーザ描画コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void UserRenderCommand::Initialize(Player& player) {
	m_targetIconInfo.resize(GetOtherPlayers(player).size());

	// 赤HPの初期化
	m_redHp = m_preHp = GetStatus(player).hp;
	m_redHpTime = 0.0f;
}

/// <summary>
/// ユーザ描画コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void UserRenderCommand::Execute(Player& player, const DX::StepTimer& timer) {
	timer;
	// ターゲットの方向を示すアイコンを更新する
	UpdateIcon(player);
	
	// 照準を更新する
	UpdateAiming(player, timer);

	// 赤HPを更新する
	UpdateRedHpBar(player, timer);
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

	// プレイヤーのモデルを描画する
	RenderPlayerModel(player, view, proj);

	// 相手プレイヤーのアイコンを表示する
	RenderEnemeyIcon(player, spriteBatch);

	// 所持エレメントを描画する
	RenderElements(player, spriteBatch);

	// HPバーを描画する
	RenderHpBar(player, spriteBatch);
	
	// SPバーを描画する
	RenderSpBar(player, spriteBatch);
	
	// 照準を描画する
	RenderAiming(player, spriteBatch);
	
}

/// <summary>
/// ターゲットの方向を示すアイコンを更新する
/// </summary>
/// <param name="player">プレイヤー</param>
void UserRenderCommand::UpdateIcon(Player& player) {
	// 相手プレイヤーが画面外にいる場合、方向を表示するためのUIの座標を計算する
	const Camera& camera = GetCamera(player);
	const TargetCamera* target_camera = dynamic_cast<const TargetCamera*>(&camera);

	// アイコンの表示フラグをリセットする
	for (std::vector<UserRenderCommand::TargetIconInfo>::iterator itr = m_targetIconInfo.begin(); itr != m_targetIconInfo.end(); ++itr) {
		itr->enable = false;
	}

	// 画角を取得して、画面外かどうかの範囲を決める
	float area_angle;
	if (target_camera) {
		area_angle = target_camera->GetFov();
	}
	else {
		// カメラがない場合、画角45°と仮定する
		area_angle = Math::QuarterPI;
	}
	const DirectX::SimpleMath::Vector3& camera_pos = camera.GetEyePosition();
	DirectX::SimpleMath::Vector3 camera_dir = camera.GetEyeVector();
	const std::vector<Player*>& other_players = GetOtherPlayers(player);

	for (unsigned int i = 0; i < other_players.size(); i++) {
		const DirectX::SimpleMath::Vector3& other_player_pos = GetTransform(*other_players[i]).GetLocalPosition();
		DirectX::SimpleMath::Vector3 other_dir = other_player_pos - camera_pos;
		float angle = std::acosf(camera_dir.Dot(other_dir) / (camera_dir.Length()*other_dir.Length()));
		// カメラの向きと敵の方向が一定の角度以内であれば描画しない
		if (angle < area_angle) {
			break;
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
		m_targetIconInfo[i].pos = CalculateIconPos(vec, screen_size, DirectX::SimpleMath::Vector2(screen_offset, screen_offset));
		// アイコンの描画を有効にする
		m_targetIconInfo[i].enable = true;
		// アイコンのキャラID情報を設定する
		m_targetIconInfo[i].charaId = GetStatus(*other_players[i]).charaId;
	}
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

/// <summary>
/// 照準を更新する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void UserRenderCommand::UpdateAiming(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const PlayerStatus& status = GetStatus(player);
	m_aimingPos = ServiceLocater<MouseWrapper>::Get()->GetPos();
	const float aiming_rotation_speed = Math::HarfPI;
	// ブースト中なら照準の回転を速くする
	if (status.isBoosting) {
		m_aimigRotation += elapsed_time * aiming_rotation_speed*2.0f;
	}
	else {
		m_aimigRotation += elapsed_time * aiming_rotation_speed;
	}
}

/// <summary>
/// 相手プレイヤーのアイコンを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::RenderEnemeyIcon(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	player;
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
	for (unsigned int i = 0; i < m_targetIconInfo.size(); ++i) {
		// 有効なアイコンのみ描画する
		if (m_targetIconInfo[i].enable) {
			spriteBatch->Draw(texture->GetResource(m_targetIconInfo[i].charaId).Get(),
				m_targetIconInfo[i].pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), 0,
				texture->GetCenter(), DirectX::SimpleMath::Vector2(0.4f, 0.4f));
		}
	}
}

/// <summary>
/// 所持エレメントを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::RenderElements(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const std::list<ElementID>& have_elements = GetHaveElements(player);
	const PlayerStatus& status = GetStatus(player);
	int i = have_elements.size() - 1;
	// 最初に取得したエレメントが左に来るように表示する
	for (std::list<ElementID>::const_reverse_iterator itr = have_elements.rbegin(); itr != have_elements.rend(); ++itr) {
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicIcon);
		spriteBatch->Draw(texture->GetResource(static_cast<int>(*itr)).Get(), DirectX::SimpleMath::Vector2(90 + i * 50.0f, 540.0f), nullptr,
			DirectX::Colors::White, 0, texture->GetCenter(), DirectX::SimpleMath::Vector2::One*(i <= status.chargeLevel ? 1.5f : 1.1f));
		--i;
	}
}

/// <summary>
/// HPバーを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::RenderHpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::HpBar);
	DirectX::SimpleMath::Vector2 pos(30.0f, 650.0f);
	DirectX::SimpleMath::Vector2 scale(0.55f, 0.5f);

	// 黒の部分を描画する
	spriteBatch->Draw(texture->GetResource(2).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// 画像の矩形を作成する
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const PlayerStatus& status = GetStatus(player);
	RECT rect{ 0,0,0,0 };
	// 赤HPの割合に応じて描画範囲を決める
	rect.right = static_cast<LONG>(size.x*m_redHp / status.maxHp);
	rect.bottom = static_cast<LONG>(size.y);

	// 赤の部分を描画する
	spriteBatch->Draw(texture->GetResource(1).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// HPの割合に応じて描画範囲を決める
	rect.right = static_cast<LONG>(size.x*status.hp / status.maxHp);

	// 緑の部分を描画する
	spriteBatch->Draw(texture->GetResource(0).Get(), pos, &rect,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

}

/// <summary>
/// SPバーを描画する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::RenderSpBar(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::SpBar);
	DirectX::SimpleMath::Vector2 pos(30.0f, 610.0f);
	DirectX::SimpleMath::Vector2 scale(0.45f, 0.3f);

	// 黒の部分を描画する
	spriteBatch->Draw(texture->GetResource(1).Get(), pos, nullptr,
		DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, scale);

	// 画像の矩形を作成する
	DirectX::SimpleMath::Vector2 size = texture->GetSize();
	const PlayerStatus& status = GetStatus(player);
	RECT rect;
	rect.left = 0; rect.top = 0;
	// SPの割合に応じて描画範囲を決める
	rect.right = static_cast<LONG>(size.x*status.sp / status.maxSp);
	rect.bottom = static_cast<LONG>(size.y);

	// SPが減少した直後は暗くする
	DirectX::SimpleMath::Color color = DirectX::Colors::White;
	if (status.spDecreaseTimer > 0.0f) {
		color = DirectX::SimpleMath::Color(0.7f, 0.7f, 0.7f, 1);
	}
	spriteBatch->Draw(texture->GetResource(0).Get(), pos, &rect,
		color, 0, DirectX::SimpleMath::Vector2::Zero, scale);
}

/// <summary>
/// 照準を描画する
/// </summary>
/// /// <param name="player">プレイヤー</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void UserRenderCommand::RenderAiming(const Player& player, DirectX::SpriteBatch* spriteBatch) const {
	// ロックオン中なら照準の色を変える
	bool lock_on = (GetStatus(player).lockOnPlayerID != -1);

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::MagicAiming);
	spriteBatch->Draw(texture->GetResource(lock_on ? 1 : 0).Get(),
		m_aimingPos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, 0.8f), m_aimigRotation,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(0.25f, 0.25f));
}

