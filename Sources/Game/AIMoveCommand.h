#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// AI�ړ��R�}���h�N���X
/// </summary>
class AIMoveCommand : public Command {
public:
	// �R���X�g���N�^
	AIMoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// AI�ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���o�ߎ���
	float                        m_totalElapsedTime;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3 m_euler;

};


#endif // !AI_MOVE_COMMAND_DEFINED
