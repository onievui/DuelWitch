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

		float rollingTime;
		float rollAngle;
		float rollMoveLength;

		struct user_param {
			float cameraRotXLimit;
			float cameraRotYLimit;
			float zoomFov;
			float zoomTime;
		};
		user_param userParam;

		struct ai_param {
			float collectElementBoostSp;
			float chaseBoostSp;
			float chaseBoostDistance;
			float evadeNearBoostSp;
			float evadeNearBoostDistance;
			float evadeFarBoostSp;
			float evadeFarBoostDistance;
		};
		ai_param aiParam;
	};
	move_param moveParam;

	struct ai_cast_param {
		float shotableAngle;
		float castDelay;
	};
	ai_cast_param aiCastparam;
};


#endif // !COMMAND_PARAMETER_DEFINED
