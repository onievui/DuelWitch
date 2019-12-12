#include "PlayerManager.h"
#include <Utils\MathUtils.h>
#include "Player.h"
#include "PlayerID.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager() {
	// プレイヤーの初期座標
	DirectX::SimpleMath::Vector3 player_pos(0, 0, -75);
	// プレイヤーの初期座標をずらすための回転を生成する
	DirectX::SimpleMath::Quaternion player_pos_rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / static_cast<int>(PlayerID::Num));

	// プレイヤーを生成する
	for (int i = 0; i < static_cast<int>(PlayerID::Num); ++i) {
		// 少しづつ回転させる
		player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
		m_players.emplace_back(std::make_unique<Player>(static_cast<PlayerID>(i), player_pos));
	}
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager() {

}

/// <summary>
/// プレイヤーマネージャを初期化する
/// </summary>
/// <param name="pMagicManager">魔法マネージャへのポインタ</param>
/// <param name="camera">カメラへのポインタ</param>
void PlayerManager::Initialize(MagicManager* pMagicManager, Camera* camera) {
	// プレイヤーを初期化する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Initialize(pMagicManager, camera, m_players);
	}
}

/// <summary>
/// プレイヤーマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void PlayerManager::Update(const DX::StepTimer& timer) {
	// 生存していないプレイヤーを削除する
	m_players.erase(std::remove_if(m_players.begin(), m_players.end(), std::mem_fn(&Player::IsDead)), m_players.end());

	// プレイヤーを更新する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->SetOtherPlayers(m_players);
		(*itr)->Update(timer);
	}

}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void PlayerManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	DirectX::SpriteBatch* spriteBatch) const {
	// プレイヤーを描画する
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, proj, spriteBatch);
	}
}


/// <summary>
/// プレイヤーを取得する
/// </summary>
/// <returns>
/// プレイヤー
/// </returns>
std::vector<std::unique_ptr<Player>>* PlayerManager::GetPlayers() {
	return &m_players;
}

/// <summary>
/// プレイヤー1が勝利しているか判定する
/// </summary>
/// <returns>
/// true : 勝利している
/// false : 勝利していない
/// </returns>
bool PlayerManager::Player1Win() {
	// 生存しているプレイヤーがいないなら（同時に倒れた場合）勝利扱いにする
	if (m_players.size() == 0) {
		return true;
	}
	// プレイヤー1だけが生存しているなら勝利
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() == PlayerID::Player1; });
}

/// <summary>
/// プレイヤー1が敗北しているか判定する
/// </summary>
/// <returns>
/// true : 敗北している
/// false : 敗北していない
/// </returns>
bool PlayerManager::Player1Lose() {
	// 生存しているプレイヤーがいないなら（同時に倒れた場合）勝利扱いにする
	if (m_players.size() == 0) {
		return false;
	}

	// プレイヤー1が生存していないなら敗北
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() != PlayerID::Player1; });
}
