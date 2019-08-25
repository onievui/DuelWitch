#pragma once
#ifndef AI_COMMAND_PARAMETER_DEFINED
#define AI_COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// AIコマンドクラス用パラメータ
/// </summary>
class AICommandParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
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
