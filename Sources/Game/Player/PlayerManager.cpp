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
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / 3);

	// プレイヤーを生成する
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player1, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player2, player_pos));
	player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
	m_players.emplace_back(std::make_unique<Player>(PlayerID::Player3, player_pos));
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
	// プレイヤーを更新する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
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
