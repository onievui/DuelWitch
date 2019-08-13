#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	MoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// 移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 総経過時間
	float                        m_totalElapsedTime;
	// オイラー角
	DirectX::SimpleMath::Vector3 m_euler;
};


#endif // !MOVE_COMMAND_DEFINED
