#pragma once
#ifndef AI_COMMAND_PARAMETER_DEFINED
#define AI_COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// AI�R�}���h�N���X�p�p�����[�^
/// </summary>
class AICommandParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	struct move_param {
		float moveSpeed;
		float moveSpeedXY;
		float rotSpeed;
		float rotZLimit;
		float rotXLimit;
		float rotYLimit;
		float lerpSpeed;
		float nearDistance;
	};
	move_param moveParam;

	struct cast_param {
		float shotableAngle;
		float castDelay;
	};
	cast_param castParam;
};

#endif // !AI_COMMAND_PARAMETER_DEFINED
