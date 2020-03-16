#pragma once
#ifndef MAGIC_PARAMETER_DEFINED
#define MAGIC_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// ���@�N���X�p�p�����[�^
/// </summary>
class MagicParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

private:
	// �p�����[�^��ǂݍ���
	bool LoadData();

public:
	std::vector<int> maxNum;

	struct normal_param {
		float power;				// �_���[�W��
		float radius;				// ���a
		float moveSpeed;			// �ړ����x
		float lockOnRotateSpeed;	// ���b�N�I�����̕����]�����x
		float lockOnTime;			// ���b�N�I�����I�����鎞��
		float lifeTime;				// ������܂ł̎���
	};
	normal_param normalParam;

	struct fire_param {
		float power;	            // �_���[�W��
		float wayAngle;	            // �e���@�̊p�x�̊Ԋu
		float radius;               // ���a
		float height;               // ����
		float moveSpeed;	        // �ړ����x
		float lockOnRotateSpeed;	// ���b�N�I�����̕����]�����x
		float lockOnTime;			// ���b�N�I�����I�����鎞��
		float lifeTime;	            // ������܂ł̎���
	};
	fire_param fireParam;

	struct freeze_param {
		float power;                // �_���[�W��
		float radius;               // ���a
		float rotateSpeed;          // ��]���x
		float rotateRadius;	        // ��]���a
		float lifeTime;             // ������܂ł̎���
	};
	freeze_param freezeParam;

	struct thunder_param {
		float power;                // �_���[�W��
		float radius;               // ���a
		float colliderRadius;       // �ǔ������蔻��p���a
		float moveSpeed;            // �ړ����x
		float chaseSpeed;           // �ǔ����x
		float chaseEndDistance;     // �ǔ����I�����鋗��
		float chaseLerpSpeed;       // �ǔ��̋���
		float chaseEndTime;         // �ǔ����I�����鎞��
		float lockOnRotateSpeed;	// ���b�N�I�����̕����]�����x
		float lockOnTime;			// ���b�N�I�����I�����鎞��
		float lifeTime;             // ������܂ł̎���
	};
	thunder_param thunderParam;

	struct thunder_strike_param {
		float power;        // �_���[�W��
		float radius;       // ���a
		float height;       // ����
		float appearPosY;	// �o�����鍂���̃I�t�Z�b�g
		float moveSpeed;    // �ړ����x
		float lifeTime;     // ������܂ł̎���
	};
	thunder_strike_param thunderStrikeParam;
};

#endif // !MAGIC_PARAMETER_DEFINED
