#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// AI�ړ��R�}���h�N���X
/// </summary>
class AIMoveCommand : public Command {
public:
	AIMoveCommand() : m_totalElapsedTime() {}

public:
	// AI�ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���o�ߎ���
	float m_totalElapsedTime;
};


#endif // !AI_MOVE_COMMAND_DEFINED