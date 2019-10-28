#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include <Game\Common\EmptyObject.h>


class PlayerTrailEffectEmitter;
class TargetCamera;


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
	// �ʏ�ړ��̏������s��
	void ExcuteMove(Player& player, const DX::StepTimer& timer);
	// ���[������̏������s��
	void ExcuteRoll(Player& player, const DX::StepTimer& timer);
	// ���[������̓��͔���
	void RollInputCheck(Player& player, const DX::StepTimer& timer);
	// �ړ��ɂ��Y�[��
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);
	// �Ə��ɂ��J���������𒲐�����
	void AdjustCamera(TargetCamera* targetCamera);

private:
	// �ړ��R�}���h�p�X�e�[�g
	enum class MoveState {
		Move,	// �ʏ�ړ�
		Roll,	// ���[�����
	};

	// ���[������Ɋւ�����
	struct RollInfo {
		// ���Ƀ��[�����鎞�̓��͗P�\����
		float                           leftGraceTime;
		// �E�Ƀ��[�����鎞�̓��͗P�\����
		float                           rightGraceTime;
		// ���[�����Ă���o�ߎ���
		float                           rollingTime;
		// ���[�����Ă������
		bool                            isRollingLeft;
		// ���[���O��Z����]��
		float                           preRotZ;
	};

private:
	// ���݂̃X�e�[�g
	MoveState                               m_state;
	// ���[���Ɋւ�����
	RollInfo                                m_rollInfo;
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
