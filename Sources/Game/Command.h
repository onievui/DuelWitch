#pragma once
#ifndef COMMAND_DEFINED
#define COMMAND_DEFINED


#include <Framework\StepTimer.h>
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "MagicManager.h"


/// <summary>
/// プレイヤーコマンドクラス
/// </summary>
class Command {
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
	Player::Status&              GetStatus(Player& player)                   { return player.m_status; }
	// プレイヤーのステータスを取得する
	const Player::Status&        GetStatus(const Player& player) const       { return player.m_status; }
	// プレイヤーの進行方向を取得する
	Player::MoveDirection&       GetMoveDirection(Player& player)            { return player.m_direction; }
	// プレイヤーの所持エレメントを取得する
	std::list<ElementID>&        GetHaveElements(Player& player)             { return player.m_haveElements; }
	// プレイヤーの所持エレメントを取得する
	const std::list<ElementID>&  GetHaveElements(const Player& player) const { return player.m_haveElements; }
	// プレイヤーの当たり判定を取得する
	const SphereCollider&        GetCollider(const Player& player) const     { return player.m_sphereCollider; }
	// カメラを取得する
	Camera&                      GetCamera(Player& player)                   { return *player.m_pCamera; }
	// 魔法マネージャを取得する
	MagicManager&                GetMagicManager(Player& player)             { return *player.m_pMagicManager; }
	// 敵プレイヤーを取得する
	const Player&                GetOtherPlayer(Player& player)              { return *player.m_pOtherPlayer; }
};


#endif // !COMMAND_DEFINED
