#include "AICommandParameter.h"
#include <Utils\JsonWrapper.h>
#include <Utils\MathUtils.h>


/// <summary>
/// �p�����[�^��ǂݍ���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
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
/// �p�����[�^���ēǂݍ��݂���
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool AICommandParameter::Reload() {
	return Load();
}

/// <summary>
/// �p�����[�^��ۑ�����
/// </summary>
/// <returns>
/// true  : ����
/// false : ���s
/// </returns>
bool AICommandParameter::Save() {
	// ������
	return false;
}
