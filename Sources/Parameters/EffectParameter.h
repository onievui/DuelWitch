#pragma once
#ifndef EFFECT_PARAMETER_DEFINED
#define EFFECT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// エフェクトクラス用パラメータ
/// </summary>
class EffectParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	std::vector<int> maxNum;

	struct player_trail_param {
		int particleNum;
		DirectX::SimpleMath::Vector3 appearPosOffset;
		float lifeTime;
		float speed;
		float acceleration;
		float apexAngle;
		float maxScale;
		float minScale;
		int maxHColor;
		int minHColor;
		int sColor;
		int vColor;
	};
	player_trail_param playerTrailParam;

	struct field_shield_param {
		int particleNum;
		float lifeTime;
		float scale;
		float scaleSpeed;
	};
	field_shield_param fieldShieldParam;

	struct normal_magic_param {
		int particleNum;
		float scale;
	};
	normal_magic_param normalMagicParam;

	struct fire_magic_param {
		int particleNum;
		float scale;
		float speed;
		float maxAccelerationX;
		float minAccelerationX;
		float accelerationYZ;
		float maxLifeTime;
		float minLifeTime;
		DirectX::SimpleMath::Color color;
	};
	fire_magic_param fireMagicParam;

	

	struct freeze_param {

	};
	freeze_param freezeParam;

	struct thunder_param {

	};
	thunder_param thunderParam;

	struct thunder_strike_param {

	};
	thunder_strike_param thunderStrikeParam;
};

#endif // !EFFECT_PARAMETER_DEFINED
