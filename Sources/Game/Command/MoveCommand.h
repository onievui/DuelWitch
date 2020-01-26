#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>
#include <Game\Object\EmptyObject.h>


class PlayerTrailEffectEmitter;
class PlayerChargeEffectEmitter;
class TargetCamera;


/// <summary>
/// �ړ��R�}���h�N���X
/// </summary>
class MoveCommand : public Command {
public:
	// �R���X�g���N�^
	MoveCommand();
	// �f�X�g���N�^
	~MoveCommand();

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
	// �N�C�b�N�^�[���̏������s��
	void ExecuteTurn(Player& player, const DX::StepTimer& timer);
	// ���[������̓��͔���
	void RollInputCheck(Player& player, const DX::StepTimer& timer);
	// �N�C�b�N�^�[���̓��͔���
	void TurnInputCheck(Player& player, const DX::StepTimer& timer);
	// �ړ��ɂ��Y�[��
	void Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting);
	// �Ə��ɂ��J���������𒲐�����
	void AdjustCamera(TargetCamera* targetCamera);

private:
	// �ړ��R�}���h�p�X�e�[�g
	enum class MoveState {
		Move,	// �ʏ�ړ�
		Roll,	// ���[�����
		Turn,   // �N�C�b�N�^�[��
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

	// �N�C�b�N�^�[���Ɋւ�����
	struct TurnInfo {
		// �N�C�b�N�^�[�����鎞�̓��͗P�\����
		float                           graceTime;
		// �N�C�b�N�^�[�����Ă���̌o�ߎ���
		float                           turningTime;
		// ��]�������
		int                             turnDirection;
		// �N�C�b�N�^�[���O��X����]��
		float                           preRotX;
		// �N�C�b�N�^�[���O��Y����]��
		float                           preRotY;
	};

private:
	// ���݂̃X�e�[�g
	MoveState                               m_state;
	// ���[���Ɋւ�����
	RollInfo                                m_rollInfo;
	// �N�C�b�N�^�[���Ɋւ�����
	TurnInfo                                m_turnInfo;
	// �J�����ւ̃|�C���^
	TargetCamera*                           m_pTargetCamera;
	// �J�����^�[�Q�b�g
	EmptyObject                             m_cameraTarget;
	// �p���u�[�X�g����
	float                                   m_boostTime;
	// �f�t�H���g�̉�p
	float                                   m_defaultFov;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3            m_euler;
	//�@�v���C���[�̋O�ՃG�t�F�N�g�ւ̃|�C���^
	PlayerTrailEffectEmitter*               m_pTrailEffect;
	// �v���C���[�̃`���[�W�G�t�F�N�g�ւ̃|�C���^
	PlayerChargeEffectEmitter*              m_pChargeEffect;
	// �G�t�F�N�g�p�̎p���N���X
	ChildTransform                          m_effectTransform;
};


#endif // !MOVE_COMMAND_DEFINED
