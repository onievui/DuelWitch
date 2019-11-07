#include "FuzzyParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool FuzzyParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/fuzzy.json")) {
		return false;
	}

	{
		JsonWrapper::value ai_move(root["AIMove"]);
		aiMoveParam.hpGapMin           = ai_move["HpGapMin"].GetNumF();
		aiMoveParam.hpGapMax           = ai_move["HpGapMax"].GetNumF();
		aiMoveParam.distanceMin        = ai_move["DistanceMin"].GetNumF();
		aiMoveParam.distanceMax        = ai_move["DistanceMax"].GetNumF();
		aiMoveParam.hasElementNumMin   = ai_move["HasElementNumMin"].GetNumF();
		aiMoveParam.hasElementNumMax   = ai_move["HasElementNumMax"].GetNumF();
		aiMoveParam.elementDistanceMin = ai_move["ElementDistanceMin"].GetNumF();
		aiMoveParam.elementDistanceMax = ai_move["ElementDistanceMax"].GetNumF();
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
bool FuzzyParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool FuzzyParameter::Save() {
	// ������
	return false;
}
