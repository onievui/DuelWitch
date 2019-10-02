#include "AICastMagicCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\CommandParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"


/// <summary>
/// AI魔法詠唱コマンドクラス
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::ai_cast_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->aiCastparam;

	// 連射を制限する
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}

	const Transform& transform = GetTransform(player);
	const DirectX::SimpleMath::Quaternion& rot = transform.GetLocalRotation();
	const DirectX::SimpleMath::Vector3& pos = transform.GetLocalPosition();
	const DirectX::SimpleMath::Vector3& target_pos = GetTransform(GetOtherPlayer(player)).GetLocalPosition();

	// 攻撃範囲を制限する
	const float shotable_angle = parameter.shotableAngle;
	DirectX::SimpleMath::Vector3 direction = target_pos - pos;
	DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rot);
	float angle = std::acosf(forward.Dot(direction) / (forward.Length()*direction.Length()));
	if (angle > shotable_angle) {
		return;
	}

	direction.Normalize();
	std::list<ElementID>& ref_have_elements = GetHaveElements(player);
	// エレメントがないなら通常魔法を発射する
	if (ref_have_elements.empty()) {
		// SPが足りているか確認する
		Player::Status& status = GetStatus(player);
		if (status.sp >= status.normalMagicSpCost) {
			status.sp -= status.normalMagicSpCost;
			GetMagicManager(player).CreateMagic(MagicID::Normal, 0, player.GetPlayerID(), pos, direction);
		}
	}
	else {
		ElementID element_id = GetHaveElements(player).front();
		GetHaveElements(player).pop_front();
		GetMagicManager(player).CreateMagic(element_id, 0, player.GetPlayerID(), pos, direction);
	}

	const float cast_delay = parameter.castDelay;
	m_waitTime = cast_delay;
}

