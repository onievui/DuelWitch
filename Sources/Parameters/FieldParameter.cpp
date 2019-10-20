#include "FieldParameter.h"
#include <Utils\ServiceLocater.h>
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
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
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool FieldParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool FieldParameter::Save() {
	// 未実装
	return false;
}

