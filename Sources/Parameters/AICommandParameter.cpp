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

	moveParam.moveSpeed = root["MoveCommand"]["MoveSpeed"].getNumF();
	moveParam.moveSpeedXY = root["MoveCommand"]["MoveSpeedXY"].getNumF();
	moveParam.rotSpeed = root["MoveCommand"]["RotSpeed"].getNumF();
	moveParam.rotZLimit = Math::Deg2Rad(root["MoveCommand"]["RotZLimit_Deg"].getNumF());
	moveParam.rotXLimit = Math::Deg2Rad(root["MoveCommand"]["RotXLimit_Deg"].getNumF());
	moveParam.rotYLimit = Math::Deg2Rad(root["MoveCommand"]["RotYLimit_Deg"].getNumF());
	moveParam.lerpSpeed = root["MoveCommand"]["LerpSpeed"].getNumF();
	moveParam.nearDistance = root["MoveCommand"]["NearDistance"].getNumF();

	castParam.shotableAngle = Math::Deg2Rad(root["CastCommand"]["ShotableAngle_Deg"].getNumF());
	castParam.castDelay = root["CastCommand"]["CastDelay"].getNumF();

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
