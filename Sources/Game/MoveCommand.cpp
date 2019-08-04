#include "MoveCommand.h"
#include <Framework\DirectX11.h>

/// <summary>
/// 移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	constexpr float moveSpeed = 16.0f;
	constexpr float moveSpeedXY = 0.3f;
	constexpr float rotSpeed = 2.0f;
	constexpr float rotZLimit = Math::QuarterPI*0.5f;
	constexpr float rotXLimit = Math::QuarterPI*0.5f;
	constexpr float rotYLimit = Math::QuarterPI*0.25f;
	constexpr float lerpSpeed = 0.040f;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();
	DirectX::SimpleMath::Vector3 rot = ref_transform.GetRotation();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);

	m_totalElapsedTime += elapsedTime;
	if (m_totalElapsedTime > 10.0f) {
		m_totalElapsedTime -= 10.0f;
		if (ref_direction == Player::MoveDirection::Forward) {
			ref_direction = Player::MoveDirection::Backward;
		}
		else {
			ref_direction = Player::MoveDirection::Forward;
		}
	}

	// 移動
	if (keyState.A || keyState.Left) {
		rot.z = Math::Lerp(rot.z, -rotZLimit, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			rot.y = Math::Lerp(rot.y, rotYLimit, lerpSpeed);
			move.x = 1.0f;
		}
		else {
			rot.y = Math::Lerp(rot.y, Math::PI + rotYLimit, lerpSpeed);
			move.x = -1.0f;
		}
	}
	else if (keyState.D || keyState.Right) {
		rot.z = Math::Lerp(rot.z, rotZLimit, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			rot.y = Math::Lerp(rot.y, -rotYLimit, lerpSpeed);
			move.x = -1.0f;
		}
		else {
			rot.y = Math::Lerp(rot.y, Math::PI - rotYLimit, lerpSpeed);
			move.x = 1.0f;
		}
	}
	//押していないときは戻す
	else {
		rot.z = Math::Lerp(rot.z, 0.0f, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			rot.y = Math::Lerp(rot.y, 0.0f, lerpSpeed);
		}
		else {
			rot.y = Math::Lerp(rot.y, Math::PI, lerpSpeed);
		}
	}

	if (keyState.W || keyState.Up) {
		rot.x = Math::Lerp(rot.x, -rotXLimit, lerpSpeed);
		move.y = 1.0f;
	}
	else if (keyState.S || keyState.Down) {
		rot.x = Math::Lerp(rot.x, rotXLimit, lerpSpeed);
		move.y = -1.0f;
	}
	//押していないときは戻す
	else {
		rot.x = Math::Lerp(rot.x, 0.0f, lerpSpeed);
	}

	//DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

	//pos += DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ*moveSpeed*elapsedTime, quaternion);
	move.Normalize();
	move *= moveSpeedXY;
	if (ref_direction == Player::MoveDirection::Forward) {
		move.z = 1.0f;
	}
	else {
		move.z = -1.0f;
	}
	pos += move * moveSpeed*elapsedTime;

	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rot);
	GetWorld(player) = ref_transform.GetMatrix();
}
