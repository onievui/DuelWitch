#include "FieldParameter.h"
#include <Utils\ServiceLocater.h>
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool FieldParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/field.json")) {
		return false;
	}

	defaultScale       = root["DefaultScale"].GetNumF();
	startScaleDownTime = root["StartScaleDownTime"].GetNumF();
	scaleDownSpeed     = root["ScaleDownSpeed"].GetNumF();
	minScale           = root["MinScale"].GetNumF();

	return true;
}

/// <summary>
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool FieldParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool FieldParameter::Save() {
	// ������
	return false;
}

