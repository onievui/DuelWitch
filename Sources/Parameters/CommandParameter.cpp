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
