#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include "EmptyObject.h"


class PlayerTrailEffectEmitter;


/// <summary>
/// �ړ��R�}���h�N���X
/// </summary>
class MoveCommand : public Command {
public:
	// �R���X�g���N�^
	MoveCommand();

public:
	// �ړ��R�}���h������������
	void Initialize(Player& player) override;
	// �ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// �ړ��ɂ��Y�[��
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);

private:
	// �J�����^�[�Q�b�g
	EmptyObject                             m_cameraTarget;
	// �p���u�[�X�g����
	float                                   m_boostTime;
	// �f�t�H���g�̉�p
	float                                   m_defaultFov;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3            m_euler;
	//�@�v���C���[�̋O�ՃG�t�F�N�g�ւ̃|�C���^
	PlayerTrailEffectEmitter*               m_pEffect;
	// �G�t�F�N�g�p�̎p���N���X
	ChildTransform                          m_effectTransform;
};


#endif // !MOVE_COMMAND_DEFINED
