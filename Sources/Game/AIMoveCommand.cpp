#include "AIMoveCommand.h"
#include <Framework\DirectX11.h>


/// <summary>
/// AI�ړ��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�^�C�}�[</param>
void AIMoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());

	constexpr float moveSpeed = 16.0f;
	constexpr float moveSpeedXY = 0.2f;
	constexpr float rotSpeed = 2.0f;
	constexpr float rotZLimit = Math::QuarterPI*0.5f;
	constexpr float rotXLimit = Math::QuarterPI*0.5f;
	constexpr float rotYLimit = Math::QuarterPI*0.25f;
	constexpr float lerpSpeed = 0.025f;

	auto& ref_transform = GetTransform(player);
	auto& ref_direction = GetMoveDirection(player);
	const auto& other_pos = GetTransform(GetOtherPlayer(player)).GetPosition();

	auto pos = ref_transform.GetPosition();
	auto rot = ref_transform.GetRotation();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);


	m_totalElapsedTime += elapsedTime;
	// 10�b�Ő܂�Ԃ�
	if (m_totalElapsedTime > 10.0f) {
		m_totalElapsedTime -= 10.0f;
		ref_direction = (ref_direction == Player::MoveDirection::Forward ? Player::MoveDirection::Backward : Player::MoveDirection::Forward);
	}

	// �ړ�
	constexpr float nearDistance = 1.8f;
	bool is_forward = ref_direction == Player::MoveDirection::Forward;
	auto distance = other_pos - pos;
	// ����Ⴂ��̏ꍇ
	if (distance.z < 0 == is_forward) {
		// ���S�ɋ߂Â�
		if (fabsf(pos.x) > 0.3f) {
			// �i�݂��������̔���
			if (pos.x > 0 == is_forward) {
				rot.z = Math::Lerp(rot.z, -rotZLimit, lerpSpeed);
				if (is_forward) {
					rot.y = Math::Lerp(rot.y, -rotYLimit, lerpSpeed);
				}
				else {
					rot.y = Math::Lerp(rot.y, Math::PI - rotYLimit, lerpSpeed);
				}
			}
			else {
				rot.z = Math::Lerp(rot.z, rotZLimit, lerpSpeed);
				if (is_forward) {
					rot.y = Math::Lerp(rot.y, rotYLimit, lerpSpeed);
				}
				else {
					rot.y = Math::Lerp(rot.y, Math::PI + rotYLimit, lerpSpeed);
				}
			}
			move.x = (pos.x > 0 ? -1.0f : 1.0f);
		}
		else {
			rot.z = Math::Lerp(rot.z, 0.0f, lerpSpeed);
			if (is_forward) {
				rot.y = Math::Lerp(rot.y, 0.0f, lerpSpeed);
			}
			else {
				rot.y = Math::Lerp(rot.y, Math::PI, lerpSpeed);
			}
		}

		if (fabsf(pos.y) > 0.5f) {
			if (pos.y > 0) {
				rot.x = Math::Lerp(rot.x, rotXLimit, lerpSpeed);
				move.y = -1.0f;
			}
			else {
				rot.x = Math::Lerp(rot.x, -rotXLimit, lerpSpeed);
				move.y = 1.0f;
			}
		}
		else {
			rot.x = Math::Lerp(rot.x, 0.0f, lerpSpeed);
		}
	}
	// ����Ⴂ�O�̏ꍇ
	else {
		if (fabsf(distance.x) < nearDistance) {
			if (distance.x < 0 == is_forward) {
				rot.z = Math::Lerp(rot.z, -rotZLimit, lerpSpeed);
				if (is_forward) {
					rot.y = Math::Lerp(rot.y, rotYLimit, lerpSpeed);
					move.x = 1.0f;
				}
				else {
					rot.y = Math::Lerp(rot.y, Math::PI + rotYLimit, lerpSpeed);
					move.x = -1.0f;
				}
			}
			else {
				rot.z = Math::Lerp(rot.z, rotZLimit, lerpSpeed);
				if (is_forward) {
					rot.y = Math::Lerp(rot.y, -rotYLimit, lerpSpeed);
					move.x = -1.0f;
				}
				else {
					rot.y = Math::Lerp(rot.y, Math::PI - rotYLimit, lerpSpeed);
					move.x = 1.0f;
				}
			}
		}
		else {
			rot.z = Math::Lerp(rot.z, 0.0f, lerpSpeed);
			if (is_forward) {
				rot.y = Math::Lerp(rot.y, 0.0f, lerpSpeed);
			}
			else {
				rot.y = Math::Lerp(rot.y, Math::PI, lerpSpeed);
			}
		}

		if (fabsf(distance.y) < nearDistance) {
			if (distance.y > 0) {
				rot.x = Math::Lerp(rot.x, rotXLimit, lerpSpeed);
				move.y = -1.0f;
			}
			else {
				rot.x = Math::Lerp(rot.x, -rotXLimit, lerpSpeed);
				move.y = 1.0f;
			}
		}
		else {
			rot.x = Math::Lerp(rot.x, 0.0f, lerpSpeed);
		}
	}
	//auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

	//pos += DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ*moveSpeed*elapsedTime, quaternion);
	move.Normalize();
	move *= moveSpeedXY;
	if (is_forward) {
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
