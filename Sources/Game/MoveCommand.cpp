#include "MoveCommand.h"
#include <Utils\MathUtils.h>
#include <Utils\JsonWrapper.h>


MoveCommand::MoveCommandData MoveCommand::s_data;


/// <summary>
/// 移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	float& moveSpeed   = s_data->moveSpeed;
	float& moveSpeedXY = s_data->moveSpeedXY;
	float& rotSpeed    = s_data->rotSpeed;
	float& rotZLimit   = s_data->rotZLimit;
	float& rotXLimit   = s_data->rotXLimit;
	float& rotYLimit   = s_data->rotYLimit;
	float& lerpSpeed   = s_data->lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();
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
		m_euler.z = Math::Lerp(m_euler.z, -rotZLimit, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, rotYLimit, lerpSpeed);
			move.x = 1.0f;
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI + rotYLimit, lerpSpeed);
			move.x = -1.0f;
		}
	}
	else if (keyState.D || keyState.Right) {
		m_euler.z = Math::Lerp(m_euler.z, rotZLimit, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, -rotYLimit, lerpSpeed);
			move.x = -1.0f;
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI - rotYLimit, lerpSpeed);
			move.x = 1.0f;
		}
	}
	//押していないときは戻す
	else {
		m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerpSpeed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerpSpeed);
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerpSpeed);
		}
	}

	if (keyState.W || keyState.Up) {
		m_euler.x = Math::Lerp(m_euler.x, -rotXLimit, lerpSpeed);
		move.y = 1.0f;
	}
	else if (keyState.S || keyState.Down) {
		m_euler.x = Math::Lerp(m_euler.x, rotXLimit, lerpSpeed);
		move.y = -1.0f;
	}
	//押していないときは戻す
	else {
		m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerpSpeed);
	}

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
bool MoveCommand::MoveCommandData::Load() {
	JsonWrapper::root root;
	if (!JsonWrapper::LoadCheck(root, L"Resources/Jsons/move_command.json")) {
		return false;
	}

	if (!m_value) {
		m_value = std::make_unique<_value>();
	}

	m_value->moveSpeed   = root["MoveSpeed"].getNum();
	m_value->moveSpeedXY = root["MoveSpeedXY"].getNum();
	m_value->rotSpeed    = root["RotSpeed"].getNum();
	m_value->rotZLimit   = Math::Deg2Rad(root["RotZLimit"].getNum());
	m_value->rotXLimit   = Math::Deg2Rad(root["RotXLimit"].getNum());
	m_value->rotYLimit   = Math::Deg2Rad(root["RotYLimit"].getNum());
	m_value->lerpSpeed   = root["LerpSpeed"].getNum();

	return true;
}

/// <summary>
/// データを開放する
/// </summary>
void MoveCommand::MoveCommandData::Dispose() {
	m_value.reset();
}
