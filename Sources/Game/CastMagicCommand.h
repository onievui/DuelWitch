#pragma once
#ifndef CAST_MAGIC_COMMAND_DEFINED
#define CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 魔法詠唱コマンドクラス
/// </summary>
class CastMagicCommand : public Command {
public:
	// コンストラクタ
	CastMagicCommand();

public:
	// 魔法詠唱コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 魔法のためのレイ用平面の作成
	DirectX::SimpleMath::Plane CreatePlaneForMagic(const Transform& transform, Player::MoveDirection direction);

private:
	// マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_mouseTracker;
};


#endif // !CAST_MAGIC_COMMAND_DEFINED
