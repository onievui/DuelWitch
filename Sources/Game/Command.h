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
	// コマンドを処理する
	virtual void Execute(Player& player, const DX::StepTimer& timer) = 0;

protected:
	// プレイヤーの姿勢を取得する
	Transform& GetTransform(Player& player) {
		return player.m_transform;
	}
	// プレイヤーの姿勢を取得する
	const Transform& GetTransform(const Player& player) {
		return player.m_transform;
	}
	// プレイヤーの行列を取得する
	DirectX::SimpleMath::Matrix& GetWorld(Player& player) {
		return player.m_world;
	}
	// プレイヤーの進行方向を取得する
	Player::MoveDirection& GetMoveDirection(Player& player) {
		return player.m_direction;
	}
	// カメラを取得する
	Camera& GetCamera(Player& player) {
		return *player.m_pCamera;
	}
	// 魔法マネージャを取得する
	MagicManager& GetMagicManager(Player& player) {
		return *player.m_pMagicManager;
	}
	// 敵プレイヤーを取得する
	const Player& GetOtherPlayer(Player& player) {
		return *player.m_otherPlayer;
	}
};


#endif // !COMMAND_DEFINED
