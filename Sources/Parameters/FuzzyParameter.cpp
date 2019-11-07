#include "FuzzyParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
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
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool FuzzyParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool FuzzyParameter::Save() {
	// 未実装
	return false;
}
