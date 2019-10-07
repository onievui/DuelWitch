#pragma once
#ifndef COMMAND_PARAMETER_DEFINED
#define COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �R�}���h�N���X�p�p�����[�^
/// </summary>
class CommandParameter : public ILoadData {
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
		float boostRotSpeed;
		float rotZLimit;
		float rotXLimit;
		float rotYLimit;
		float lerpSpeed;

		float cameraRotXLimit;
		float cameraRotYLimit;
		float zoomFov;
		float zoomTime;

		float nearDistance;
	};
	move_param moveParam;

	struct ai_cast_param {
		float shotableAngle;
		float castDelay;
	};
	ai_cast_param aiCastparam;
};


#endif // !COMMAND_PARAMETER_DEFINED
