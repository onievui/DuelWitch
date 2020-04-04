#include "EffectParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool EffectParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/effect.json")) {
		return false;
	}

	maxNum.resize(root["_MaxNum_Size"].GetNumI());
	for (unsigned int i = 0; i < maxNum.size(); ++i) {
		maxNum[i] = root["MaxNum"][i].GetNumI();
	}

	{
		JsonWrapper::value player_trail(root["PlayerTrail"]);
		playerTrailParam.particleNum     = player_trail["ParticleNum"].GetNumI();
		playerTrailParam.appearPosOffset = player_trail["AppearPosOffset"].GetVector3();
		playerTrailParam.lifeTime        = player_trail["LifeTime"].GetNumF();
		playerTrailParam.speed           = player_trail["Speed"].GetNumF();
		playerTrailParam.acceleration    = player_trail["Acceleration"].GetNumF();
		playerTrailParam.apexAngle       = Math::Deg2Rad(player_trail["ApexAngle_Deg"].GetNumF());
		playerTrailParam.maxScale        = player_trail["MaxScale"].GetNumF();
		playerTrailParam.minScale        = player_trail["MinScale"].GetNumF();
		playerTrailParam.maxHColor       = player_trail["MaxHColor"].GetNumI();
		playerTrailParam.minHColor       = player_trail["MinHColor"].GetNumI();
		playerTrailParam.sColor          = player_trail["SColor"].GetNumI();
		playerTrailParam.vColor          = player_trail["VColor"].GetNumI();
	}
	{
		JsonWrapper::value player_charge(root["PlayerCharge"]);
		playerChargeParam.particleNum     = player_charge["ParticleNum"].GetNumI();
		playerChargeParam.appearPosOffset = player_charge["AppearPosOffset"].GetVector3();
		playerChargeParam.lifeTime        = player_charge["LifeTime"].GetNumF();
		playerChargeParam.maxScale        = player_charge["MaxScale"].GetNumF();
		playerChargeParam.minScale        = player_charge["MinScale"].GetNumF();
		playerChargeParam.colorCharge0    = player_charge["ColorCharge0"].GetVector4();
		playerChargeParam.colorCharge1    = player_charge["ColorCharge1"].GetVector4();
		playerChargeParam.colorCharge2    = player_charge["ColorCharge2"].GetVector4();
	}
	{
		JsonWrapper::value field_shield(root["FieldShield"]);
		fieldShieldParam.particleNum = field_shield["ParticleNum"].GetNumI();
		fieldShieldParam.lifeTime    = field_shield["LifeTime"].GetNumF();
		fieldShieldParam.scale       = field_shield["Scale"].GetNumF();
		fieldShieldParam.scaleSpeed  = field_shield["ScaleSpeed"].GetNumF();
	}
	{
		JsonWrapper::value hit(root["Hit"]);
		hitParam.particleNum = hit["ParticleNum"].GetNumI();
		hitParam.lifeTime    = hit["LifeTime"].GetNumF();
		hitParam.scale       = hit["Scale"].GetNumF();
	}
	{
		JsonWrapper::value normal_magic(root["NormalMagic"]);
		normalMagicParam.particleNum = normal_magic["ParticleNum"].GetNumI();
		normalMagicParam.scale       = normal_magic["Scale"].GetNumF();
	}
	{
		JsonWrapper::value fire_magic(root["FireMagic"]);
		fireMagicParam.particleNum      = fire_magic["ParticleNum"].GetNumI();
		fireMagicParam.scale            = fire_magic["Scale"].GetNumF();
		fireMagicParam.speed            = fire_magic["Speed"].GetNumF();
		fireMagicParam.maxAccelerationX = fire_magic["MaxAccelerationX"].GetNumF();
		fireMagicParam.minAccelerationX = fire_magic["MinAccelerationX"].GetNumF();
		fireMagicParam.accelerationYZ   = fire_magic["AccelerationYZ"].GetNumF();
		fireMagicParam.maxLifeTime      = fire_magic["MaxLifeTime"].GetNumF();
		fireMagicParam.minLifeTime      = fire_magic["MinLifeTime"].GetNumF();
		fireMagicParam.color            = fire_magic["Color"].GetVector4();
	}
	{
		JsonWrapper::value thunder_strike_magic(root["ThunderStrikeMagic"]);
		thunderStrikeMagicParam.particleNum = thunder_strike_magic["ParticleNum"].GetNumI();
		thunderStrikeMagicParam.scale       = thunder_strike_magic["Scale"].GetNumF();
	}
	{
		JsonWrapper::value effective(root["Effective"]);
		effectiveParam.particleNum    = effective["ParticleNum"].GetNumI();
		effectiveParam.lifeTime       = effective["LifeTime"].GetNumF();
		effectiveParam.maxRotateSpeed = effective["MaxRotateSpeed"].GetNumF();
		effectiveParam.minRotateSpeed = effective["MinRotateSpeed"].GetNumF();
		effectiveParam.maxScale       = effective["MaxScale"].GetNumF();
		effectiveParam.minScale       = effective["MinScale"].GetNumF();
		effectiveParam.maxSpeed       = effective["MaxSpeed"].GetNumF();
		effectiveParam.minSpeed       = effective["MinSpeed"].GetNumF();
	}
	{
		JsonWrapper::value death(root["Death"]);
		deathParam.particleNum = death["ParticleNum"].GetNumI();
		deathParam.lifeTime    = death["LifeTime"].GetNumF();
		deathParam.maxScale    = death["MaxScale"].GetNumF();
		deathParam.minScale    = death["MinScale"].GetNumF();
		deathParam.maxSpeed    = death["MaxSpeed"].GetNumF();
		deathParam.minSpeed    = death["MinSpeed"].GetNumF();
		deathParam.blinkSpeed  = death["BlinkSpeed"].GetNumF();
	}

	return true;
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool EffectParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool EffectParameter::Save() {
	// 未実装
	return false;
}

