#include "CharaStatusParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool CharaStatusParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/chara_status.json")) {
		return false;
	}

	{
		JsonWrapper::value chara1(root["Chara1"]);
		chara1Param.maxHp                 = chara1["MaxHp"].GetNumF();
		chara1Param.maxSp                 = chara1["MaxSp"].GetNumF();
		chara1Param.spRecoverySpeed       = chara1["SpRecoverySpeed"].GetNumF();
		chara1Param.normalMagicSpCost     = chara1["NormalMagicSpCost"].GetNumF();
		chara1Param.boostSpeedRate        = chara1["BoostSpeedRate"].GetNumF();
		chara1Param.boostSpCost           = chara1["BoostSpCost"].GetNumF();
		chara1Param.firstChargeTime       = chara1["FirstChargeTime"].GetNumF();
		chara1Param.secoundChargeTime     = chara1["SecoundChargeTime"].GetNumF();
		chara1Param.fireMagicPowerRate    = chara1["FireMagicPowerRate"].GetNumF();
		chara1Param.freezeMagicPowerRate  = chara1["FreezeMagicPowerRate"].GetNumF();
		chara1Param.thunderMagicPowerRate = chara1["ThunderMagicPowerRate"].GetNumF();
	}

	return true;
}

/// <summary>
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool CharaStatusParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool CharaStatusParameter::Save() {
	// ������
	return false;
}
