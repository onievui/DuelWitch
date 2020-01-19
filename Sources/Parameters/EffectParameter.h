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
	// エフェクトの最大種類数
	std::vector<int> maxNum;

	// プレイヤーの軌跡エフェクト
	struct player_trail_param {
		int particleNum;                                // パーティクル数
		DirectX::SimpleMath::Vector3 appearPosOffset;	// 出現位置のオフセット
		float lifeTime;                                 // パーティクルが出現してから消えるまでの時間
		float speed;                                    // パーティクルの速度
		float acceleration;                             // パーティクルの加速度
		float apexAngle;                                // パーティクルの最大角度
		float maxScale;	                                // パーティクルの最大サイズ
		float minScale;	                                // パーティクルの最小サイズ
		int maxHColor;	                                // パーティクルの最大色相
		int minHColor;	                                // パーティクルの最小色相
		int sColor;		                                // パーティクルの彩度
		int vColor;		                                // パーティクルの明度
	};
	player_trail_param playerTrailParam;

	// プレイヤーのチャージエフェクト
	struct player_charge_param {
		int particleNum;                                // パーティクル数
		DirectX::SimpleMath::Vector3 appearPosOffset;	// 出現位置のオフセット
		float lifeTime;                                 // パーティクルが出現してから消えるまでの時間
		float maxScale;	                                // パーティクルの最大サイズ
		float minScale;	                                // パーティクルの最小サイズ
		DirectX::SimpleMath::Color colorCharge0;	    // パーティクルの色(チャージ0段階)
		DirectX::SimpleMath::Color colorCharge1;	    // パーティクルの色(チャージ1段階)
		DirectX::SimpleMath::Color colorCharge2;	    // パーティクルの色(チャージ2段階)
	};
	player_charge_param playerChargeParam;

	// フィールドとの衝突エフェクト
	struct field_shield_param {
		int particleNum;	// パーティクル数
		float lifeTime;		// パーティクルが消えるまでの時間
		float scale;		// パーティクルのサイズ
		float scaleSpeed;	// パーティクルの速度
	};
	field_shield_param fieldShieldParam;

	// 魔法のヒットエフェクト
	struct hit_param {
		int particleNum;	// パーティクル数
		float lifeTime;		// パーティクルが消えるまでの時間
		float scale;		// パーティクルのサイズ
	};
	hit_param hitParam;

	// 通常魔法エフェクト
	struct normal_magic_param {
		int particleNum;	// パーティクル数
		float scale;		// パーティクルのサイズ
	};
	normal_magic_param normalMagicParam;

	// 炎魔法エフェクト
	struct fire_magic_param {
		int particleNum;					// パーティクル数
		float scale;						// パーティクルのサイズ
		float speed;						// パーティクルの速度
		float maxAccelerationX;				// パーティクルのX方向の最大加速度
		float minAccelerationX;				// パーティクルのX方向の最小化速度
		float accelerationYZ;				// パーティクルのYZ方向の加速度
		float maxLifeTime;					// パーティクルが消えるまでの最大時間
		float minLifeTime;					// パーティクルが消えるまでの最小時間
		DirectX::SimpleMath::Color color;	// パーティクルの色
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

	// 落雷魔法エフェクト
	struct thunder_strike_param {
		int particleNum;	// パーティクル数
		float scale;		// パーティクルのサイズ
	};
	thunder_strike_param thunderStrikeMagicParam;

	// 撃破エフェクト
	struct death_param {
		int particleNum;                                // パーティクル数
		float lifeTime;                                 // パーティクルが出現してから消えるまでの時間
		float maxScale;	                                // パーティクルの最大サイズ
		float minScale;	                                // パーティクルの最小サイズ
		float maxSpeed;                                 // パーティクルの最大速度
		float minSpeed;                                 // パーティクルの最小速度
		float blinkSpeed;                               // パーティクルの点滅速度
	};
	death_param deathParam;
};

#endif // !EFFECT_PARAMETER_DEFINED
