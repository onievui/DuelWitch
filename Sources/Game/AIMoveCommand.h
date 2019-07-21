#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// AI移動コマンドクラス
/// </summary>
class AIMoveCommand : public Command {
public:
	AIMoveCommand() : m_totalElapsedTime() {}

public:
	// AI移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// 総経過時間
	float m_totalElapsedTime;
};


#endif // !AI_MOVE_COMMAND_DEFINED
