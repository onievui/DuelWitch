#pragma once
#ifndef PLAYER_STATUS_DEFINED
#define PLAYER_STATUS_DEFINED



/// <summary>
/// �v���C���[�X�e�[�^�X
/// </summary>
struct PlayerStatus {
	// �O���f�[�^
	// �ő�HP
	float                             maxHp;
	// HP
	float                             hp;
	// �O�t���[����HP
	float                             preHp;
	// �ő�SP
	float                             maxSp;
	// SP
	float                             sp;
	// �O�t���[����SP
	float                             preSp;
	// SP�񕜑��x
	float                             spRecoverySpeed;
	// �ʏ햂�@�̏���SP
	float                             normalMagicSpCost;
	// �u�[�X�g���x�{��
	float                             boostSpeedRate;
	// �u�[�X�g����SP
	float                             boostSpCost;
	// ���[���������SP
	float                             rollSpCost;
	// 1�i�K�ڂ̃`���[�W�^�C��
	float                             firstChargeTime;
	// 2�i�K�ڂ̃`���[�W�^�C��
	float                             secoundChargeTime;
	// �����@�̃_���[�W�{��
	float                             fireMagicPowerRate;
	// �X���@�̃_���[�W�{��
	float                             freezeMagicPowerRate;
	// �����@�̃_���[�W�{��
	float                             thunderMagicPowerRate;

	// �����f�[�^
	// �L�����N�^�[ID
	int                               charaId;
	// �_���[�W�㖳�G���ԃ^�C�}�[
	float                             damageTimer;
	// SP�����^�C�}�[
	float                             spDecreaseTimer;
	// �u�[�X�g�����ǂ���
	bool                              isBoosting;
	// �`���[�W�����ǂ���
	bool                              isCharging;
	// �`���[�W�i�K
	int                               chargeLevel;
	// �r���\���ǂ���
	bool                              canCast;
};


#endif // !PLAYER_STATUS_DEFINED
