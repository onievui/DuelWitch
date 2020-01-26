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

	{
		int size = root["_CharaNum"].GetNumI();
		charaParams.resize(size);
		JsonWrapper::value chara(root["Charas"]);
		for (int i = 0; i < size; ++i) {
			charaParams[i].maxHp                 = chara[i]["MaxHp"].GetNumF();
			charaParams[i].maxSp                 = chara[i]["MaxSp"].GetNumF();
			charaParams[i].spRecoverySpeed       = chara[i]["SpRecoverySpeed"].GetNumF();
			charaParams[i].normalMagicSpCost     = chara[i]["NormalMagicSpCost"].GetNumF();
			charaParams[i].boostSpeedRate        = chara[i]["BoostSpeedRate"].GetNumF();
			charaParams[i].boostSpCost           = chara[i]["BoostSpCost"].GetNumF();
			charaParams[i].rollSpCost            = chara[i]["RollSpCost"].GetNumF();
			charaParams[i].quickTurnSpCost       = chara[i]["QuickTurnSpCost"].GetNumF();
			charaParams[i].firstChargeTime       = chara[i]["FirstChargeTime"].GetNumF();
			charaParams[i].secoundChargeTime     = chara[i]["SecoundChargeTime"].GetNumF();
			charaParams[i].fireMagicPowerRate    = chara[i]["FireMagicPowerRate"].GetNumF();
			charaParams[i].freezeMagicPowerRate  = chara[i]["FreezeMagicPowerRate"].GetNumF();
			charaParams[i].thunderMagicPowerRate = chara[i]["ThunderMagicPowerRate"].GetNumF();
		}
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
