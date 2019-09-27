#include "EffectParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
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

	return true;
}

/// <summary>
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool EffectParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool EffectParameter::Save() {
	// ������
	return false;
}

