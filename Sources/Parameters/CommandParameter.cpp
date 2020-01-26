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

	{
		JsonWrapper::value move_command(root["MoveCommand"]);
		moveParam.moveSpeed       = move_command["MoveSpeed"].GetNumF();
		moveParam.rotSpeed        = Math::Deg2Rad(move_command["RotSpeed_Deg"].GetNumF());
		moveParam.boostRotSpeed   = Math::Deg2Rad(move_command["BoostRotSpeed_Deg"].GetNumF());
		moveParam.rotZLimit       = Math::Deg2Rad(move_command["RotZLimit_Deg"].GetNumF());
		moveParam.rotXLimit       = Math::Deg2Rad(move_command["RotXLimit_Deg"].GetNumF());
		moveParam.rotYLimit       = Math::Deg2Rad(move_command["RotYLimit_Deg"].GetNumF());
		moveParam.lerpSpeed       = move_command["LerpSpeed"].GetNumF();

		moveParam.rollingTime     = move_command["RollingTime"].GetNumF();
		moveParam.rollAngle       = Math::Deg2Rad(move_command["RollAngle_Deg"].GetNumF());
		moveParam.rollMoveLength  = move_command["RollMoveLength"].GetNumF();

		moveParam.quickTurnTime  = move_command["QuickTurnTime"].GetNumF();

		{
			JsonWrapper::value user(move_command["User"]);
			moveParam.userParam.cameraRotXLimit = Math::Deg2Rad(user["CameraRotXLimit_Deg"].GetNumF());
			moveParam.userParam.cameraRotYLimit = Math::Deg2Rad(user["CameraRotYLimit_Deg"].GetNumF());
			moveParam.userParam.zoomFov         = Math::Deg2Rad(user["ZoomFov_Deg"].GetNumF());
			moveParam.userParam.zoomTime        = user["ZoomTime"].GetNumF();
		}

		{
			JsonWrapper::value ai(move_command["AI"]);
			moveParam.aiParam.collectElementBoostSp  = ai["CollectElementBoostSp"].GetNumF();
			moveParam.aiParam.chaseBoostSp           = ai["ChaseBoostSp"].GetNumF();
			moveParam.aiParam.chaseBoostDistance     = ai["ChaseBoostDistance"].GetNumF();
			moveParam.aiParam.evadeNearBoostSp       = ai["EvadeNearBoostSp"].GetNumF();
			moveParam.aiParam.evadeNearBoostDistance = ai["EvadeNearBoostDistance"].GetNumF();
			moveParam.aiParam.evadeFarBoostSp        = ai["EvadeFarBoostSp"].GetNumF();
			moveParam.aiParam.evadeFarBoostDistance  = ai["EvadeFarBoostDistance"].GetNumF();
		}
	}
	{
		JsonWrapper::value ai_cast_command(root["AICastCommand"]);
		aiCastparam.shotableAngle = Math::Deg2Rad(ai_cast_command["ShotableAngle_Deg"].GetNumF());
		aiCastparam.castDelay     = ai_cast_command["CastDelay"].GetNumF();
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
