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

	normalMagicParam.particleNum    = root["NormalMagic"]["ParticleNum"].GetNumI();
	normalMagicParam.scale          = root["NormalMagic"]["Scale"].GetNumF();

	fireMagicParam.particleNum      = root["FireMagic"]["ParticleNum"].GetNumI();
	fireMagicParam.scale            = root["FireMagic"]["Scale"].GetNumF();
	fireMagicParam.speed            = root["FireMagic"]["Speed"].GetNumF();
	fireMagicParam.maxAccelerationX = root["FireMagic"]["MaxAccelerationX"].GetNumF();
	fireMagicParam.minAccelerationX = root["FireMagic"]["MinAccelerationX"].GetNumF();
	fireMagicParam.accelerationYZ   = root["FireMagic"]["AccelerationYZ"].GetNumF();
	fireMagicParam.maxLifeTime      = root["FireMagic"]["MaxLifeTime"].GetNumF();
	fireMagicParam.minLifeTime      = root["FireMagic"]["MinLifeTime"].GetNumF();
	fireMagicParam.color            = root["FireMagic"]["Color"].GetVector4();

	playerTrailParam.particleNum     = root["PlayerTrail"]["ParticleNum"].GetNumI();
	playerTrailParam.appearPosOffset = root["PlayerTrail"]["AppearPosOffset"].GetVector3();
	playerTrailParam.lifeTime        = root["PlayerTrail"]["LifeTime"].GetNumF();
	playerTrailParam.speed           = root["PlayerTrail"]["Speed"].GetNumF();
	playerTrailParam.acceleration    = root["PlayerTrail"]["Acceleration"].GetNumF();
	playerTrailParam.apexAngle       = Math::Deg2Rad(root["PlayerTrail"]["ApexAngle_Deg"].GetNumF());
	playerTrailParam.maxScale        = root["PlayerTrail"]["MaxScale"].GetNumF();
	playerTrailParam.minScale        = root["PlayerTrail"]["MinScale"].GetNumF();
	playerTrailParam.maxHColor       = root["PlayerTrail"]["MaxHColor"].GetNumI();
	playerTrailParam.minHColor       = root["PlayerTrail"]["MinHColor"].GetNumI();
	playerTrailParam.sColor          = root["PlayerTrail"]["SColor"].GetNumI();
	playerTrailParam.vColor          = root["PlayerTrail"]["VColor"].GetNumI();


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

