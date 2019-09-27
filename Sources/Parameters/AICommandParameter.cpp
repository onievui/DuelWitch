#include "AICommandParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool AICommandParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/ai_command.json")) {
		return false;
	}

	moveParam.moveSpeed = root["MoveCommand"]["MoveSpeed"].GetNumF();
	moveParam.moveSpeedXY = root["MoveCommand"]["MoveSpeedXY"].GetNumF();
	moveParam.rotSpeed = root["MoveCommand"]["RotSpeed"].GetNumF();
	moveParam.rotZLimit = Math::Deg2Rad(root["MoveCommand"]["RotZLimit_Deg"].GetNumF());
	moveParam.rotXLimit = Math::Deg2Rad(root["MoveCommand"]["RotXLimit_Deg"].GetNumF());
	moveParam.rotYLimit = Math::Deg2Rad(root["MoveCommand"]["RotYLimit_Deg"].GetNumF());
	moveParam.lerpSpeed = root["MoveCommand"]["LerpSpeed"].GetNumF();
	moveParam.nearDistance = root["MoveCommand"]["NearDistance"].GetNumF();

	castParam.shotableAngle = Math::Deg2Rad(root["CastCommand"]["ShotableAngle_Deg"].GetNumF());
	castParam.castDelay = root["CastCommand"]["CastDelay"].GetNumF();

	return true;
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool AICommandParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool AICommandParameter::Save() {
	// 未実装
	return false;
}
