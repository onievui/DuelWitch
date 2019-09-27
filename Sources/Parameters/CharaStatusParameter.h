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
	struct chara1_param {
		float maxHp;
		float maxSp;
		float spRecoverySpeed;
		float normalMagicSpCost;
		float boostSpeedRate;
		float boostSpCost;
	};
	chara1_param chara1Param;

};

#endif // !CHARA_STATUS_PARAMETER_DEFINED
