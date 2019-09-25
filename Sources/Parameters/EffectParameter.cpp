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

	maxNum.resize(root["_MaxNum_Size"].getNumI());
	for (unsigned int i = 0; i < maxNum.size(); ++i) {
		maxNum[i] = root["MaxNum"][i].getNumI();
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

