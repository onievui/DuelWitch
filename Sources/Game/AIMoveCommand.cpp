#include "AIMoveCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\JsonWrapper.h>


LoadDataHolder<AIMoveCommand::AIMoveCommandData, LoadDataID::PlayScene> AIMoveCommand::s_data;


/// <summary>
/// AI移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void AIMoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	const float& moveSpeed   = s_data->moveSpeed;
	const float& moveSpeedXY = s_data->moveSpeedXY;
	const float& rotSpeed    = s_data->rotSpeed;
	const float& rotZLimit   = s_data->rotZLimit;
	const float& rotXLimit   = s_data->rotXLimit;
	const float& rotYLimit   = s_data->rotYLimit;
	const float& lerpSpeed   = s_data->lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);
	const DirectX::SimpleMath::Vector3& other_pos = GetTransform(GetOtherPlayer(player)).GetPosition();

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);

	m_totalElapsedTime += elapsedTime;
	// 10秒で折り返し
	if (m_totalElapsedTime > 10.0f) {
		m_totalElapsedTime -= 10.0f;
		ref_direction = (ref_direction == Player::MoveDirection::Forward ? Player::MoveDirection::Backward : Player::MoveDirection::Forward);
	}

	// 移動
	const float& nearDistance = s_data->nearDistance;
	bool is_forward = ref_direction == Player::MoveDirection::Forward;
	DirectX::SimpleMath::Vector3 distance = other_pos - pos;
	// すれ違い後の場合
	if (distance.z < 0 == is_forward) {
		// 中心に近づく
		if (fabsf(pos.x) > 0.3f) {
			// 進みたい方向の判定
			if (pos.x > 0 == is_forward) {
				m_euler.z = Math::Lerp(m_euler.z, -rotZLimit, lerpSpeed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, -rotYLimit, lerpSpeed);
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI - rotYLimit, lerpSpeed);
				}
			}
			else {
				m_euler.z = Math::Lerp(m_euler.z, rotZLimit, lerpSpeed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, rotYLimit, lerpSpeed);
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI + rotYLimit, lerpSpeed);
				}
			}
			move.x = (pos.x > 0 ? -1.0f : 1.0f);
		}
		else {
			m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerpSpeed);
			if (is_forward) {
				m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerpSpeed);
			}
			else {
				m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerpSpeed);
			}
		}

		if (fabsf(pos.y) > 0.5f) {
			if (pos.y > 0) {
				m_euler.x = Math::Lerp(m_euler.x, rotXLimit, lerpSpeed);
				move.y = -1.0f;
			}
			else {
				m_euler.x = Math::Lerp(m_euler.x, -rotXLimit, lerpSpeed);
				move.y = 1.0f;
			}
		}
		else {
			m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerpSpeed);
		}
	}
	// すれ違い前の場合
	else {
		if (fabsf(distance.x) < nearDistance) {
			if (distance.x < 0 == is_forward) {
				m_euler.z = Math::Lerp(m_euler.z, -rotZLimit, lerpSpeed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, rotYLimit, lerpSpeed);
					move.x = 1.0f;
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI + rotYLimit, lerpSpeed);
					move.x = -1.0f;
				}
			}
			else {
				m_euler.z = Math::Lerp(m_euler.z, rotZLimit, lerpSpeed);
				if (is_forward) {
					m_euler.y = Math::Lerp(m_euler.y, -rotYLimit, lerpSpeed);
					move.x = -1.0f;
				}
				else {
					m_euler.y = Math::Lerp(m_euler.y, Math::PI - rotYLimit, lerpSpeed);
					move.x = 1.0f;
				}
			}
		}
		else {
			m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerpSpeed);
			if (is_forward) {
				m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerpSpeed);
			}
			else {
				m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerpSpeed);
			}
		}

		if (fabsf(distance.y) < nearDistance) {
			if (distance.y > 0) {
				m_euler.x = Math::Lerp(m_euler.x, rotXLimit, lerpSpeed);
				move.y = -1.0f;
			}
			else {
				m_euler.x = Math::Lerp(m_euler.x, -rotXLimit, lerpSpeed);
				move.y = 1.0f;
			}
		}
		else {
			m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerpSpeed);
		}
	}

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
	ref_transform.SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));
	GetWorld(player) = ref_transform.GetMatrix();
}


/// <summary>
/// データを読み込む
/// </summary>
/// <returns>
/// true  : 成功
/// false : 失敗
/// </returns>
bool AIMoveCommand::AIMoveCommandData::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/ai_command.json")) {
		return false;
	}

	moveSpeed    = root["MoveCommand"]["MoveSpeed"].getNum();
	moveSpeedXY  = root["MoveCommand"]["MoveSpeedXY"].getNum();
	rotSpeed     = root["MoveCommand"]["RotSpeed"].getNum();
	rotZLimit    = Math::Deg2Rad(root["MoveCommand"]["RotZLimit_Deg"].getNum());
	rotXLimit    = Math::Deg2Rad(root["MoveCommand"]["RotXLimit_Deg"].getNum());
	rotYLimit    = Math::Deg2Rad(root["MoveCommand"]["RotYLimit_Deg"].getNum());
	lerpSpeed    = root["MoveCommand"]["LerpSpeed"].getNum();
	nearDistance = root["MoveCommand"]["NearDistance"].getNum();

	return true;
}


