#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// �ړ��R�}���h�N���X
/// </summary>
class MoveCommand : public Command {
public:
	MoveCommand() : m_totalElapsedTime() {}

public:
	// �ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���o�ߎ���
	float m_totalElapsedTime;

};


#endif // !MOVE_COMMAND_DEFINED
