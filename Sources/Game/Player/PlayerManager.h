#pragma once


#include <Framework\StepTimer.h>


class Player;
class MagicManager;
class Camera;


/// <summary>
/// プレイヤーマネージャクラス
/// </summary>
class PlayerManager {
public:
	// コンストラクタ
	PlayerManager();
	// デストラクタ
	~PlayerManager();

public:
	// プレイヤーマネージャを初期化する
	void Initialize(MagicManager* pMagicManager, Camera* camera);
	// プレイヤーマネージャを更新する
	void Update(const DX::StepTimer& timer);
	// プレイヤーを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const;

public:
	// プレイヤーを取得する
	std::vector<std::unique_ptr<Player>>* GetPlayers();
	// プレイヤー1が勝利しているか判定する
	bool Player1Win();
	// プレイヤー1が敗北しているか判定する
	bool Player1Lose();

private:
	// プレイヤー
	std::vector<std::unique_ptr<Player>> m_players;
	//
};