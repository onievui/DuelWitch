#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// 移動コマンドクラス
/// </summary>
class MoveCommand : public Command {
public:
	// 移動コマンドを処理する
	void Execute(Player& player, const DX::StepTimer& timer) override;

};


#endif // !MOVE_COMMAND_DEFINED
