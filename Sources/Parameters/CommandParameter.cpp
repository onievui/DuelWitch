#include "CommandParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// パラメータを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CommandParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/player_command.json")) {
		return false;
	}

	moveParam.moveSpeed = root["MoveCommand"]["MoveSpeed"].GetNumF();
	moveParam.moveSpeedXY = root["MoveCommand"]["MoveSpeedXY"].GetNumF();
	moveParam.rotSpeed = root["MoveCommand"]["RotSpeed"].GetNumF();
	moveParam.rotZLimit = Math::Deg2Rad(root["MoveCommand"]["RotZLimit_Deg"].GetNumF());
	moveParam.rotXLimit = Math::Deg2Rad(root["MoveCommand"]["RotXLimit_Deg"].GetNumF());
	moveParam.rotYLimit = Math::Deg2Rad(root["MoveCommand"]["RotYLimit_Deg"].GetNumF());
	moveParam.lerpSpeed = root["MoveCommand"]["LerpSpeed"].GetNumF();

	moveParam.cameraRotXLimit = Math::Deg2Rad(root["MoveCommand"]["CameraRotXLimit_Deg"].GetNumF());
	moveParam.cameraRotYLimit = Math::Deg2Rad(root["MoveCommand"]["CameraRotYLimit_Deg"].GetNumF());

	moveParam.nearDistance = root["MoveCommand"]["NearDistance"].GetNumF();

	aiCastparam.shotableAngle = Math::Deg2Rad(root["AICastCommand"]["ShotableAngle_Deg"].GetNumF());
	aiCastparam.castDelay = root["AICastCommand"]["CastDelay"].GetNumF();

	return true;
}

/// <summary>
/// パラメータを再読み込みする
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CommandParameter::Reload() {
	return Load();
}

/// <summary>
/// パラメータを保存する
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool CommandParameter::Save() {
	// 未実装
	return false;
}
