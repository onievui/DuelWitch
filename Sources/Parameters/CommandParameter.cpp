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

	moveSpeed = root["MoveCommand"]["MoveSpeed"].getNumF();
	moveSpeedXY = root["MoveCommand"]["MoveSpeedXY"].getNumF();
	rotSpeed = root["MoveCommand"]["RotSpeed"].getNumF();
	rotZLimit = Math::Deg2Rad(root["MoveCommand"]["RotZLimit_Deg"].getNumF());
	rotXLimit = Math::Deg2Rad(root["MoveCommand"]["RotXLimit_Deg"].getNumF());
	rotYLimit = Math::Deg2Rad(root["MoveCommand"]["RotYLimit_Deg"].getNumF());
	lerpSpeed = root["MoveCommand"]["LerpSpeed"].getNumF();
	cameraRotXLimit = Math::Deg2Rad(root["MoveCommand"]["CameraRotXLimit_Deg"].getNumF());
	cameraRotYLimit = Math::Deg2Rad(root["MoveCommand"]["CameraRotYLimit_Deg"].getNumF());

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
