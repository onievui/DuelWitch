#pragma once
#ifndef CHARA_STATUS_PARAMETER_DEFINED
#define CHARA_STATUS_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �L�����X�e�[�^�X�p�p�����[�^
/// </summary>
class CharaStatusParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	// �L�����N�^�[�̃p�����[�^
	struct chara_param {
		float maxHp;                    // �ő�HP
		float maxSp;                    // �ő�SP
		float spRecoverySpeed;          // SP�񕜑��x
		float normalMagicSpCost;	    // �ʏ햂�@�̏���SP
		float boostSpeedRate;	        // �u�[�X�g�ړ����̑��x�{��
		float boostSpCost;              // �u�[�X�g�ړ��̏���SP(�b)
		float rollSpCost;               // ���[������̏���SP
		float quickTurnSpCost;          // �N�C�b�N�^�[���̏���SP
		float firstChargeTime;          // 1�i�K�`���[�W�܂ł̎���
		float secoundChargeTime;        // 2�i�K�`���[�W�܂ł̎���
		float fireMagicPowerRate;       // �����@�̍U���{��
		float freezeMagicPowerRate;     // �X���@�̍U���{��
		float thunderMagicPowerRate;	// �����@�̍U���{��
	};
	std::vector<chara_param> charaParams;

};

#endif // !CHARA_STATUS_PARAMETER_DEFINED
