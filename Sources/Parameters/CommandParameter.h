#pragma once
#ifndef COMMAND_PARAMETER_DEFINED
#define COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// コマンドクラス用パラメータ
/// </summary>
class CommandParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	// 移動コマンド用パラメータ
	struct move_param {
		float moveSpeed;
		float rotSpeed;
		float boostRotSpeed;
		float rotZLimit;
		float rotXLimit;
		float rotYLimit;
		float lerpSpeed;

		float rollingTime;
		float rollAngle;
		float rollMoveLength;

		float quickTurnTime;

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
