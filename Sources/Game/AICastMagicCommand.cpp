#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\JsonWrapper.h>
#include "MagicFactory.h"


LoadDataHolder<AICastMagicCommand::AICastMagicCommandData, LoadDataID::PlayScene> AICastMagicCommand::s_data;


/// <summary>
/// AI魔法詠唱コマンドクラス
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	// 連射を制限する
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}

	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Quaternion& rot = transform.GetRotation();
	const DirectX::SimpleMath::Vector3& pos = transform.GetPosition();
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(GetOtherPlayer(player)).GetPosition();

	// 攻撃範囲を制限する
	const float& shotableAngle = s_data->shotableAngle;
	DirectX::SimpleMath::Vector3 direction = target_pos - pos;
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rot);
	float angle = std::acosf(forward.Dot(direction) / (forward.Length()*direction.Length()));
	if (angle > shotableAngle) {
		return;
	}

	direction.Normalize();
	std::list<ElementID>& ref_have_elements = GetHaveElements(player);
	// エレメントがないなら通常魔法を発射する
	if (ref_have_elements.empty()) {
		GetMagicManager(player).CreateMagic(MagicID::Normal, player.GetPlayerID(), pos, direction);
	}
	else {
		ElementID element_id = GetHaveElements(player).front();
		GetHaveElements(player).pop_front();
		GetMagicManager(player).CreateMagic(element_id, player.GetPlayerID(), pos, direction);
	}

	const float& castDelay = s_data->castDelay;
	m_waitTime = castDelay;
}


/// <summary>
/// データを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool AICastMagicCommand::AICastMagicCommandData::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/ai_command.json")) {
		return false;
	}

	shotableAngle = Math::Deg2Rad(root["CastCommand"]["ShotableAngle_Deg"].getNum());
	castDelay     = root["CastCommand"]["CastDelay"].getNum();

	return true;
}
