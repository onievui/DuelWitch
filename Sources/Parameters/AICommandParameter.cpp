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

	{
		JsonWrapper::value move_command(root["MoveCommand"]);
		moveParam.moveSpeed    = move_command["MoveSpeed"].GetNumF();
		moveParam.moveSpeedXY  = move_command["MoveSpeedXY"].GetNumF();
		moveParam.rotSpeed     = move_command["RotSpeed"].GetNumF();
		moveParam.rotZLimit    = Math::Deg2Rad(move_command["RotZLimit_Deg"].GetNumF());
		moveParam.rotXLimit    = Math::Deg2Rad(move_command["RotXLimit_Deg"].GetNumF());
		moveParam.rotYLimit    = Math::Deg2Rad(move_command["RotYLimit_Deg"].GetNumF());
		moveParam.lerpSpeed    = move_command["LerpSpeed"].GetNumF();
		moveParam.nearDistance = move_command["NearDistance"].GetNumF();
	}
	{
		JsonWrapper::value cast_command(root["CastCommand"]);
		castParam.shotableAngle = Math::Deg2Rad(cast_command["ShotableAngle_Deg"].GetNumF());
		castParam.castDelay     = cast_command["CastDelay"].GetNumF();
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
