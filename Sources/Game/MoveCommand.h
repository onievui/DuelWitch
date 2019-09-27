#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include "EmptyObject.h"


/// <summary>
/// �ړ��R�}���h�N���X
/// </summary>
class MoveCommand : public Command {
public:
	// �R���X�g���N�^
	MoveCommand();

public:
	// �ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// �ړ��ɂ��Y�[��
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);

private:
	// ���o�ߎ���
	float                        m_totalElapsedTime;
	// �p���u�[�X�g����
	float                        m_boostTime;
	// �f�t�H���g�̉�p
	float                        m_defaultFov;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3 m_euler;
	// �J�����^�[�Q�b�g
	EmptyObject                  m_cameraTarget;

};


#endif // !MOVE_COMMAND_DEFINED
