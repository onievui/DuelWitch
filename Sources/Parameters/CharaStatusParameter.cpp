#include "CharaStatusParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CharaStatusParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/chara_status.json")) {
		return false;
	}

	chara1Param.maxHp             = root["Chara1"]["MaxHp"].GetNumF();
	chara1Param.maxSp             = root["Chara1"]["MaxSp"].GetNumF();
	chara1Param.spRecoverySpeed   = root["Chara1"]["SpRecoverySpeed"].GetNumF();
	chara1Param.normalMagicSpCost = root["Chara1"]["NormalMagicSpCost"].GetNumF();
	chara1Param.boostSpeedRate    = root["Chara1"]["BoostSpeedRate"].GetNumF();
	chara1Param.boostSpCost       = root["Chara1"]["BoostSpCost"].GetNumF();

	return true;
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CharaStatusParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CharaStatusParameter::Save() {
	// 未実装
	return false;
}
