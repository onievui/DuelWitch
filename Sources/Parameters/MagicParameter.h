#pragma once
#ifndef MAGIC_PARAMETER_DEFINED
#define MAGIC_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// 魔法クラス用パラメータ
/// </summary>
class MagicParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

private:
	// パラメータを読み込む
	bool LoadData();

public:
	std::vector<int> maxNum;

	struct normal_param {
		float power;
		float radius;
		float moveSpeed;
		float lifeTime;
	};
	normal_param normalParam;

	struct fire_param {
		float power;
		float wayAngle;
		float radius;
		float height;
		float moveSpeed;
		float lifeTime;
	};
	fire_param fireParam;

	struct freeze_param {
		float power;
		float radius;
		float lifeTime;
		float rotateSpeed;
		float rotateRadius;
	};
	freeze_param freezeParam;

	struct thunder_param {
		float power;
		float radius;
		float colliderRadius;
		float moveSpeed;
		float chaseSpeed;
		float chaseEndDistance;
		float chaseLerpSpeed;
		float chaseEndTime;
		float lifeTime;
	};
	thunder_param thunderParam;

	struct thunder_strike_param {
		float power;
		float radius;
		float height;
		float appearPosY;
		float moveSpeed;
		float lifeTime;
	};
	thunder_strike_param thunderStrikeParam;
};

#endif // !MAGIC_PARAMETER_DEFINED
