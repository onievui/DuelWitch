#include "MoveCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CommandParameter.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "TargetCamera.h"
#include "EffectManager.h"
#include "EffectID.h"
#include "PlayerTrailEffectEmitter.h"


/// <summary>
/// コンストラクタ
/// </summary>
MoveCommand::MoveCommand()
	: m_cameraTarget()
	, m_boostTime()
	, m_euler()
	, m_pEffect()
	, m_effectTransform(nullptr) {
}

/// <summary>
/// 移動コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void MoveCommand::Initialize(Player& player) {
	Camera& ref_camera = GetCamera(player);
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	// ターゲットカメラでない場合は処理をしない
	if (!target_camera) {
		return;
	}
	// 追従するオブジェクトが存在しない場合はターゲットを設定する
	if (!target_camera->HasTargetObject()) {
		target_camera->SetTargetObject(&m_cameraTarget);
	}

	// 初期の画角を記憶する
	m_defaultFov = ref_camera.GetFov();

	// プレイヤーの軌跡エフェクトを生成する
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	m_effectTransform.SetParent(&GetTransform(player));
	m_effectTransform.SetPosition(parameter.appearPosOffset);
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&m_effectTransform);
	m_pEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"プレイヤーの軌跡エフェクトの生成に失敗しました");
	}
}

/// <summary>
/// 移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	DirectX::Keyboard::State key_state = key_tracker->GetLastState();
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed    = parameter.moveSpeed;
	const float move_speed_xy = parameter.moveSpeedXY;
	const float rot_z_limit   = parameter.rotZLimit;
	const float rot_x_limit   = parameter.rotXLimit;
	const float rot_y_limit   = parameter.rotYLimit;
	const float lerp_speed    = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	Player::MoveDirection& ref_direction = GetMoveDirection(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);


	if (ref_direction == Player::MoveDirection::Forward && pos.z > 160.0f) {
		ref_direction = Player::MoveDirection::Backward;
	}
	else if (ref_direction == Player::MoveDirection::Backward && pos.z < -4.0f) {
		ref_direction = Player::MoveDirection::Forward;
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

	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	Player::Status& ref_status = GetStatus(player);
	// 右クリックしていて、残りSPが10％以上ならブースト移動
	if (mouse->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::ButtonState::HELD &&
		ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SPを減らす
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		ref_status.isBoosting = true;
		Zoom(GetCamera(player), timer, true);
	}
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
		Zoom(GetCamera(player), timer, false);
		
	}


	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));
	GetWorld(player) = ref_transform.GetMatrix();

	// 照準のある方へカメラを少し向ける
	int width = ServiceLocater<DirectX11>::Get()->GetWidth();
	int height = ServiceLocater<DirectX11>::Get()->GetHeight();
	const float camera_rot_x_limit = parameter.cameraRotXLimit;
	const float camera_rot_y_limit = parameter.cameraRotYLimit;
	DirectX::SimpleMath::Vector2 camera_rot(
		(mouse->GetPos().y - height / 2) / height * camera_rot_y_limit,
		-(mouse->GetPos().x - width / 2) / width * camera_rot_x_limit
	);
	DirectX::SimpleMath::Matrix target_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(camera_rot.y, camera_rot.x, 0.0f);
	m_cameraTarget.GetMatrixRef() = target_matrix * GetWorld(player);
	
}

/// <summary>
/// 移動によるズーム
/// </summary>
/// <param name="camera">カメラ</param>
/// <param name="timer">ステップタイマー</param>
/// <param name="isBoosting">ブーストしているかどうか</param>
void MoveCommand::Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const float zoom_fov = Math::PI / 18;
	const float zoom_time = 0.5f;

	if (isBoosting) {
		//画角を狭くする
		if (m_boostTime < zoom_time) {
			m_boostTime = std::min(m_boostTime + elapsed_time, zoom_time);
			float fov = m_defaultFov + m_boostTime / zoom_time * zoom_fov;
			camera.SetFov(fov);
		}
	}
	else {
		//画角を元に戻す
		if (m_boostTime > 0.0f) {
			m_boostTime = std::max(m_boostTime - elapsed_time, 0.0f);
			float fov = m_defaultFov + m_boostTime / zoom_time * zoom_fov;
			camera.SetFov(fov);
		}
	}
}

