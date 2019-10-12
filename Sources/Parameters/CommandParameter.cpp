#include "CommandParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool CommandParameter::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/player_command.json")) {
		return false;
	}

	{
		JsonWrapper::value move_command(root["MoveCommand"]);
		moveParam.moveSpeed       = move_command["MoveSpeed"].GetNumF();
		moveParam.moveSpeedXY     = move_command["MoveSpeedXY"].GetNumF();
		moveParam.rotSpeed        = Math::Deg2Rad(move_command["RotSpeed_Deg"].GetNumF());
		moveParam.boostRotSpeed   = Math::Deg2Rad(move_command["BoostRotSpeed_Deg"].GetNumF());
		moveParam.rotZLimit       = Math::Deg2Rad(move_command["RotZLimit_Deg"].GetNumF());
		moveParam.rotXLimit       = Math::Deg2Rad(move_command["RotXLimit_Deg"].GetNumF());
		moveParam.rotYLimit       = Math::Deg2Rad(move_command["RotYLimit_Deg"].GetNumF());
		moveParam.lerpSpeed       = move_command["LerpSpeed"].GetNumF();

		moveParam.rollingTime     = move_command["RollingTime"].GetNumF();
		moveParam.rollAngle       = Math::Deg2Rad(move_command["RollAngle_Deg"].GetNumF());
		moveParam.rollMoveLength  = move_command["RollMoveLength"].GetNumF();

		moveParam.cameraRotXLimit = Math::Deg2Rad(move_command["CameraRotXLimit_Deg"].GetNumF());
		moveParam.cameraRotYLimit = Math::Deg2Rad(move_command["CameraRotYLimit_Deg"].GetNumF());
		moveParam.zoomFov         = Math::Deg2Rad(move_command["ZoomFov_Deg"].GetNumF());
		moveParam.zoomTime        = move_command["ZoomTime"].GetNumF();

		moveParam.nearDistance    = move_command["NearDistance"].GetNumF();
	}
	{
		JsonWrapper::value ai_cast_command(root["AICastCommand"]);
		aiCastparam.shotableAngle = Math::Deg2Rad(ai_cast_command["ShotableAngle_Deg"].GetNumF());
		aiCastparam.castDelay     = ai_cast_command["CastDelay"].GetNumF();
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
bool CommandParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool CommandParameter::Save() {
	// ������
	return false;
}
