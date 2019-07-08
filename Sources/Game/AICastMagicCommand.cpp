#include "AICastMagicCommand.h"


/// <summary>
/// AI魔法詠唱コマンドクラス
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AICastMagicCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());
	if (m_waitTime > 0) {
		m_waitTime -= elapsedTime;
		return;
	}
	auto& ref_transform = GetTransform(player);
	const auto& rot = ref_transform.GetRotation();
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	auto direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, quaternion);
	direction.Normalize();
	GetMagicManager(player).CreateMagic(MagicFactory::MagicID::Fire, player.GetPlayerID(), ref_transform.GetPosition(), direction);

	constexpr float castDelay = 2.0f;
	m_waitTime = castDelay;
}
