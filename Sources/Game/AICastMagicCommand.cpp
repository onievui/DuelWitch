#include "AICastMagicCommand.h"
#include <Framework\DirectX11.h>


/// <summary>
/// AI魔法詠唱コマンドクラス
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());
	// 連射を制限する
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}

	const auto& transform = GetTransform(player);
	const auto& rot = transform.GetRotation();
	const auto& pos = transform.GetPosition();
	const auto& target_pos = GetTransform(GetOtherPlayer(player)).GetPosition();

	// 攻撃範囲を制限する
	constexpr float shotable_angle = Math::PI / 3;
	auto direction = target_pos - pos;
	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, quaternion);
	float angle = std::acosf(forward.Dot(direction) / (forward.Length()*direction.Length()));
	if (angle > shotable_angle) {
		return;
	}

	direction.Normalize();
	GetMagicManager(player).CreateMagic(MagicFactory::MagicID::Fire, player.GetPlayerID(), pos, direction);

	constexpr float castDelay = 2.0f;
	m_waitTime = castDelay;
}
