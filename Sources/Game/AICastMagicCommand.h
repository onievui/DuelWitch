#pragma once
#ifndef AI_CAST_MAGIC_COMMAND_DEFINED
#define AI_CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// AI���@�r���R�}���h�N���X
/// </summary>
class AICastMagicCommand : public Command {
public:
	// AI���@�r���R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���̍U���܂ł̑҂�����
	float m_waitTime;
};


#endif // !AI_CAST_MAGIC_COMMAND_DEFINED
