#pragma once
#ifndef COMMAND_DEFINED
#define COMMAND_DEFINED


#include <Framework\StepTimer.h>
#include <Game\Object\Transform.h>
#include <Game\Player\Player.h>
#include <Game\Camera\Camera.h>
#include <Game\Magic\MagicManager.h>


/// <summary>
/// プレイヤーコマンドクラス
/// </summary>
class Command {
public:
	// デストラクタ
	virtual ~Command() = default;

public:
	// コマンドを初期化する
	virtual void Initialize(Player& player) { player; }
	// コマンドを処理する
	virtual void Execute(Player& player, const DX::StepTimer& timer) = 0;

protected:
	// プレイヤーの姿勢を取得する
	Transform&                   GetTransform(Player& player)                { return player.m_transform; }
	// プレイヤーの姿勢を取得する
	const Transform&             GetTransform(const Player& player) const    { return player.m_transform; }
	// プレイヤーの行列を取得する
	DirectX::SimpleMath::Matrix& GetWorld(Player& player)                    { return player.m_world; }
	// プレイヤーのステータスを取得する
	PlayerStatus&                GetStatus(Player& player)                   { return player.m_status; }
	// プレイヤーのステータスを取得する
	const PlayerStatus&          GetStatus(const Player& player) const       { return player.m_status; }
	// プレイヤーの所持エレメントを取得する
	std::list<ElementID>&        GetHaveElements(Player& player)             { return player.m_haveElements; }
	// プレイヤーの所持エレメントを取得する
	const std::list<ElementID>&  GetHaveElements(const Player& player) const { return player.m_haveElements; }
	// プレイヤーの当たり判定を取得する
	const SphereCollider&        GetCollider(const Player& player) const     { return player.m_sphereCollider; }
	// カメラを取得する
	Camera&                      GetCamera(Player& player)                   { return *player.m_pCamera; }
	// カメラを取得する
	const Camera&                GetCamera(const Player& player) const       { return *player.m_pCamera; }
	// 魔法マネージャを取得する
	MagicManager&                GetMagicManager(Player& player)             { return *player.m_pMagicManager; }
	// 敵プレイヤーを取得する
	const std::vector<Player*>&  GetOtherPlayers(Player& player)             { return player.m_pOtherPlayers; }
};


#endif // !COMMAND_DEFINED
