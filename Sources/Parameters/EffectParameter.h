#pragma once
#ifndef EFFECT_PARAMETER_DEFINED
#define EFFECT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �G�t�F�N�g�N���X�p�p�����[�^
/// </summary>
class EffectParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	// �G�t�F�N�g�̍ő��ސ�
	std::vector<int> maxNum;

	// �v���C���[�̋O�ՃG�t�F�N�g
	struct player_trail_param {
		int particleNum;                                // �p�[�e�B�N����
		DirectX::SimpleMath::Vector3 appearPosOffset;	// �o���ʒu�̃I�t�Z�b�g
		float lifeTime;                                 // �p�[�e�B�N�����o�����Ă��������܂ł̎���
		float speed;                                    // �p�[�e�B�N���̑��x
		float acceleration;                             // �p�[�e�B�N���̉����x
		float apexAngle;                                // �p�[�e�B�N���̍ő�p�x
		float maxScale;	                                // �p�[�e�B�N���̍ő�T�C�Y
		float minScale;	                                // �p�[�e�B�N���̍ŏ��T�C�Y
		int maxHColor;	                                // �p�[�e�B�N���̍ő�F��
		int minHColor;	                                // �p�[�e�B�N���̍ŏ��F��
		int sColor;		                                // �p�[�e�B�N���̍ʓx
		int vColor;		                                // �p�[�e�B�N���̖��x
	};
	player_trail_param playerTrailParam;

	// �v���C���[�̃`���[�W�G�t�F�N�g
	struct player_charge_param {
		int particleNum;                                // �p�[�e�B�N����
		DirectX::SimpleMath::Vector3 appearPosOffset;	// �o���ʒu�̃I�t�Z�b�g
		float lifeTime;                                 // �p�[�e�B�N�����o�����Ă��������܂ł̎���
		float maxScale;	                                // �p�[�e�B�N���̍ő�T�C�Y
		float minScale;	                                // �p�[�e�B�N���̍ŏ��T�C�Y
		DirectX::SimpleMath::Color colorCharge0;	    // �p�[�e�B�N���̐F(�`���[�W0�i�K)
		DirectX::SimpleMath::Color colorCharge1;	    // �p�[�e�B�N���̐F(�`���[�W1�i�K)
		DirectX::SimpleMath::Color colorCharge2;	    // �p�[�e�B�N���̐F(�`���[�W2�i�K)
	};
	player_charge_param playerChargeParam;

	// �t�B�[���h�Ƃ̏Փ˃G�t�F�N�g
	struct field_shield_param {
		int particleNum;	// �p�[�e�B�N����
		float lifeTime;		// �p�[�e�B�N����������܂ł̎���
		float scale;		// �p�[�e�B�N���̃T�C�Y
		float scaleSpeed;	// �p�[�e�B�N���̑��x
	};
	field_shield_param fieldShieldParam;

	// ���@�̃q�b�g�G�t�F�N�g
	struct hit_param {
		int particleNum;	// �p�[�e�B�N����
		float lifeTime;		// �p�[�e�B�N����������܂ł̎���
		float scale;		// �p�[�e�B�N���̃T�C�Y
	};
	hit_param hitParam;

	// �ʏ햂�@�G�t�F�N�g
	struct normal_magic_param {
		int particleNum;	// �p�[�e�B�N����
		float scale;		// �p�[�e�B�N���̃T�C�Y
	};
	normal_magic_param normalMagicParam;

	// �����@�G�t�F�N�g
	struct fire_magic_param {
		int particleNum;					// �p�[�e�B�N����
		float scale;						// �p�[�e�B�N���̃T�C�Y
		float speed;						// �p�[�e�B�N���̑��x
		float maxAccelerationX;				// �p�[�e�B�N����X�����̍ő�����x
		float minAccelerationX;				// �p�[�e�B�N����X�����̍ŏ������x
		float accelerationYZ;				// �p�[�e�B�N����YZ�����̉����x
		float maxLifeTime;					// �p�[�e�B�N����������܂ł̍ő厞��
		float minLifeTime;					// �p�[�e�B�N����������܂ł̍ŏ�����
		DirectX::SimpleMath::Color color;	// �p�[�e�B�N���̐F
	};
	fire_magic_param fireMagicParam;

	//struct freeze_param {
	//
	//};
	//freeze_param freezeMagicParam;

	//struct thunder_param {
	//
	//};
	//thunder_param thunderMagicParam;

	// �������@�G�t�F�N�g
	struct thunder_strike_param {
		int particleNum;	// �p�[�e�B�N����
		float scale;		// �p�[�e�B�N���̃T�C�Y
	};
	thunder_strike_param thunderStrikeMagicParam;

	// ���j�G�t�F�N�g
	struct death_param {
		int particleNum;                                // �p�[�e�B�N����
		float lifeTime;                                 // �p�[�e�B�N�����o�����Ă��������܂ł̎���
		float maxScale;	                                // �p�[�e�B�N���̍ő�T�C�Y
		float minScale;	                                // �p�[�e�B�N���̍ŏ��T�C�Y
		float maxSpeed;                                 // �p�[�e�B�N���̍ő呬�x
		float minSpeed;                                 // �p�[�e�B�N���̍ŏ����x
		float blinkSpeed;                               // �p�[�e�B�N���̓_�ő��x
	};
	death_param deathParam;
};

#endif // !EFFECT_PARAMETER_DEFINED
