#include "MoveCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CommandParameter.h>
#include "PlayParameterLoader.h"
#include "TargetCamera.h"


/// <summary>
/// コンストラクタ
/// </summary>
MoveCommand::MoveCommand()
	: m_totalElapsedTime()
	, m_euler()
	, m_cameraTarget() {
}

/// <summary>
/// 移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">タイマー</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	DirectX::Keyboard::State key_state = key_tracker->GetLastState();
	const CommandParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter();

	const float move_speed    = parameter->moveSpeed;
	const float move_speed_xy = parameter->moveSpeedXY;
	const float rot_z_limit   = parameter->rotZLimit;
	const float rot_x_limit   = parameter->rotXLimit;
	const float rot_y_limit   = parameter->rotYLimit;
	const float lerp_speed    = parameter->lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);

	// １０秒で折り返す
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
	if (key_state.A || key_state.Left) {
		m_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, rot_y_limit, lerp_speed);
			move.x = 1.0f;
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI + rot_y_limit, lerp_speed);
			move.x = -1.0f;
		}
	}
	else if (key_state.D || key_state.Right) {
		m_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, -rot_y_limit, lerp_speed);
			move.x = -1.0f;
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI - rot_y_limit, lerp_speed);
			move.x = 1.0f;
		}
	}
	//押していないときは戻す
	else {
		m_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed);
		if (ref_direction == Player::MoveDirection::Forward) {
			m_euler.y = Math::Lerp(m_euler.y, 0.0f, lerp_speed);
		}
		else {
			m_euler.y = Math::Lerp(m_euler.y, Math::PI, lerp_speed);
		}
	}

	if (key_state.W || key_state.Up) {
		m_euler.x = Math::Lerp(m_euler.x, -rot_x_limit, lerp_speed);
		move.y = 1.0f;
	}
	else if (key_state.S || key_state.Down) {
		m_euler.x = Math::Lerp(m_euler.x, rot_x_limit, lerp_speed);
		move.y = -1.0f;
	}
	//押していないときは戻す
	else {
		m_euler.x = Math::Lerp(m_euler.x, 0.0f, lerp_speed);
	}

	move.Normalize();
	move *= move_speed_xy;
	if (ref_direction == Player::MoveDirection::Forward) {
		move.z = 1.0f;
	}
	else {
		move.z = -1.0f;
	}
	pos += move * move_speed*elapsedTime;

	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));
	GetWorld(player) = ref_transform.GetMatrix();

	// 照準のある方へカメラを少し向ける
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	// ターゲットカメラでない場合は処理をしない
	if (!target_camera) {
		return;
	}
	// 追従するオブジェクトが存在しない場合はターゲットを設定する
	if (!target_camera->HasTargetObject()) {
		target_camera->SetTargetObject(&m_cameraTarget);
	}
	const MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	int width = ServiceLocater<DirectX11>::Get()->GetWidth();
	int height = ServiceLocater<DirectX11>::Get()->GetHeight();
	const float camera_rot_x_limit = parameter->cameraRotXLimit;
	const float camera_rot_y_limit = parameter->cameraRotYLimit;
	DirectX::SimpleMath::Vector2 camera_rot(
		(mouse->GetPos().y - height / 2) / height * camera_rot_y_limit,
		-(mouse->GetPos().x - width / 2) / width * camera_rot_x_limit
	);
	DirectX::SimpleMath::Matrix target_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(camera_rot.y, camera_rot.x, 0.0f);
	m_cameraTarget.GetMatrixRef() = target_matrix * GetWorld(player);
}

