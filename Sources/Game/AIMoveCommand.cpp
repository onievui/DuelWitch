#include "AIMoveCommand.h"
#include <Framework\DirectX11.h>


/// <summary>
/// AI移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AIMoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());

	constexpr float moveSpeed = 16.0f;
	constexpr float rotSpeed = 2.0f;
	constexpr float rotZLimit = Math::QuarterPI*0.5f;
	constexpr float rotXLimit = Math::QuarterPI*0.5f;
	constexpr float rotYLimit = Math::QuarterPI*0.25f;
	constexpr float lerpSpeed = 0.025f;

	auto& ref_transform = GetTransform(player);
	const auto& ref_direction = GetMoveDirection(player);
	const auto& other_pos = GetTransform(GetOtherPlayer(player)).GetPosition();

	auto pos = ref_transform.GetPosition();
	auto rot = ref_transform.GetRotation();

	// 移動
	constexpr float nearDistance = 2.0f;
	auto distance = other_pos - pos;
	if (std::fabsf(distance.x) < nearDistance) {
		if (distance.x > 0) {
			rot.z = Math::Lerp(rot.z, -rotZLimit, lerpSpeed);
			if (ref_direction == Player::MoveDirection::Forward) {
				rot.y = Math::Lerp(rot.y, rotYLimit, lerpSpeed);
			}
			else {
				rot.y = Math::Lerp(rot.y, Math::PI + rotYLimit, lerpSpeed);
			}
		}
		else {
			rot.z = Math::Lerp(rot.z, rotZLimit, lerpSpeed);
			if (ref_direction == Player::MoveDirection::Forward) {
				rot.y = Math::Lerp(rot.y, -rotYLimit, lerpSpeed);
			}
			else {
				rot.y = Math::Lerp(rot.y, Math::PI - rotYLimit, lerpSpeed);
			}
		}
	}
	else {
		rot.z = Math::Lerp(rot.z, 0.0f, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			rot.y = Math::Lerp(rot.y, 0.0f, lerpSpeed);
		}
		else {
			rot.y = Math::Lerp(rot.y, Math::PI, lerpSpeed);
		}
	}

	if (std::fabsf(distance.y) < nearDistance) {
		if (distance.y > 0) {
			rot.x = Math::Lerp(rot.x, rotXLimit, lerpSpeed);
		}
		else {
			rot.x = Math::Lerp(rot.x, -rotXLimit, lerpSpeed);
		}
	}
	else {
		rot.x = Math::Lerp(rot.x, 0.0f, lerpSpeed);
	}

	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

	pos += DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ*moveSpeed*elapsedTime, quaternion);

	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rot);
	GetWorld(player) = ref_transform.GetMatrix();
}
