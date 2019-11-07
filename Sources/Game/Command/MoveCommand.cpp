#include "MoveCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CommandParameter.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera/TargetCamera.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectID.h>
#include <Game\Effect\PlayerTrailEffectEmitter.h>


/// <summary>
/// コンストラクタ
/// </summary>
MoveCommand::MoveCommand()
	: m_state(MoveState::Move)
	, m_rollInfo()
	, m_cameraTarget()
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

	// カメラの追従先を設定する
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	if (target_camera) {
		target_camera->SetTargetObject(&m_cameraTarget);
	}

	// 初期の画角を記憶する
	m_defaultFov = ref_camera.GetFov();

	// 向きを初期化する
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

	// ロールに関する情報を初期化する
	m_rollInfo.leftGraceTime = 0.0f;
	m_rollInfo.rightGraceTime = 0.0f;
	m_rollInfo.rollingTime = 0.0f;
	m_rollInfo.isRollingLeft = true;

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
	// 状態によって処理を分ける
	switch (m_state) {
	// 通常移動
	case MoveCommand::MoveState::Move:
		ExcuteMove(player, timer);
		break;
	// ロール回避
	case MoveCommand::MoveState::Roll:
		ExcuteRoll(player, timer);
		break;
	default:
		ErrorMessage(L"移動コマンドの状態が不正です");
		break;
	}

	// 照準のある方へカメラを少し向ける
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	if (target_camera) {
		AdjustCamera(target_camera);
	}

}

/// <summary>
/// 通常移動の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::ExcuteMove(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	DirectX::Keyboard::State key_state = key_tracker->GetLastState();
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed = parameter.moveSpeed;
	const float rot_z_limit = parameter.rotZLimit;
	const float rot_x_limit = parameter.rotXLimit;
	const float lerp_speed = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	PlayerStatus& ref_status = GetStatus(player);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	// 回転の変化量
	DirectX::SimpleMath::Vector3 change_euler;
	// 回転量
	float rot_speed = (ref_status.isBoosting ? parameter.boostRotSpeed : parameter.rotSpeed);

	// 左右移動
	if (key_state.A || key_state.Left) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed * elapsed_time;
	}
	else if (key_state.D || key_state.Right) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed * elapsed_time;
	}
	//押していないときは戻す
	else {
		change_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed) - m_euler.z;
	}

	// 上下移動
	if (key_state.W || key_state.Up) {
		change_euler.x = -rot_speed * elapsed_time;
	}
	else if (key_state.S || key_state.Down) {
		change_euler.x = rot_speed * elapsed_time;
	}

	// 斜め移動の場合
	if (!Math::Equal0(change_euler.x) && !Math::Equal0(change_euler.y)) {
		change_euler /= std::sqrtf(2);
	}

	// 回転を計算する
	m_euler += change_euler;
	m_euler.x = Math::Clamp(m_euler.x, -rot_x_limit, rot_x_limit);
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);

	// 移動方向を計算する
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	
	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	// 右クリックしていて、残りSPが10％以上ならブースト移動
	if (mouse->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::ButtonState::HELD &&
		ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SPを減らす
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		ref_status.isBoosting = true;
	}
	// 通常移動
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
	}

	// カメラのズームを制御する
	Zoom(GetCamera(player), timer, ref_status.isBoosting);

	// 移動と回転を反映してワールド行列を求めておく
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// カメラの追従先を更新する
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.GetTransformRef().SetRotation(rotation);
	m_cameraTarget.Update(timer);

	// ロール回避の入力判定を行う
	RollInputCheck(player, timer);

}

/// <summary>
/// ロール回避の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::ExcuteRoll(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed = parameter.moveSpeed;
	const float rolling_time = parameter.rollingTime;
	const float roll_angle = parameter.rollAngle;
	const float roll_move_length = parameter.rollMoveLength;

	PlayerStatus& ref_status = GetStatus(player);
	Transform& ref_transform = GetTransform(player);
	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	m_rollInfo.rollingTime += elapsed_time;
	DirectX::SimpleMath::Vector3 roll_move(0, 0, 0);

	float t = m_rollInfo.rollingTime / rolling_time;
	float t2 = (m_rollInfo.rollingTime - elapsed_time) / rolling_time;
	t = t * (2 - t);
	t2 = t2 * (2 - t2);
	// 左にロールする場合
	if (m_rollInfo.isRollingLeft) {
		// 回転させる
		m_euler.z = m_rollInfo.preRotZ - roll_angle * t;
		if (m_euler.z < -Math::PI) {
			m_euler.z += Math::PI2;
		}
		// 移動量を求める
		roll_move = DirectX::SimpleMath::Vector3::Right * (t - t2)*roll_move_length;
	}
	// 右にロールする場合
	else {
		// 回転させる
		m_euler.z = m_rollInfo.preRotZ + roll_angle * t;
		if (m_euler.z > Math::PI) {
			m_euler.z -= Math::PI2;
		}
		// 移動量を求める
		roll_move = DirectX::SimpleMath::Vector3::Left * (t - t2)*roll_move_length;
	}

	// 回転量と移動量を計算する
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);
	DirectX::SimpleMath::Quaternion rotation_xy = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, 0);
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	roll_move = DirectX::SimpleMath::Vector3::Transform(roll_move, rotation_xy);
	pos += move * move_speed*elapsed_time + roll_move;

	// カメラのズームを制御する
	ref_status.isBoosting = false;
	Zoom(GetCamera(player), timer, false);

	// 回転と移動を反映する
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// カメラの追従先を更新する
	// 回転は変更しない
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.Update(timer);

	// 回転が終わったら元の状態に戻す
	if (m_rollInfo.rollingTime > rolling_time) {
		// 入力をリセットする
		m_rollInfo.leftGraceTime = 0.0f;
		m_rollInfo.rightGraceTime = 0.0f;

		m_state = MoveState::Move;
	}
}

/// <summary>
/// ロール回避の入力判定を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::RollInputCheck(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	PlayerStatus& ref_status = GetStatus(player);

	// タイマーを進める
	m_rollInfo.leftGraceTime -= elapsed_time;
	m_rollInfo.rightGraceTime -= elapsed_time;

	// SPが足りない時は判定しない
	if (ref_status.sp < ref_status.rollSpCost) {
		return;
	}

	// ロール回避の入力判定を行う
	const float grace_time = 0.2f;
	// 左にロールする
	if (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::A) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Left)) {
		// 素早く2回入力した時に判定する
		if (m_rollInfo.leftGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = true;
			m_rollInfo.preRotZ = m_euler.z;
			// SPを消費する
			ref_status.sp -= ref_status.rollSpCost;
		}
		m_rollInfo.leftGraceTime = grace_time;
	}
	
	// 右にロールする
	if (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::D) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Right)) {
		// 素早く2回入力した時に判定する
		if (m_rollInfo.rightGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = false;
			m_rollInfo.preRotZ = m_euler.z;
			// SPを消費する
			ref_status.sp -= ref_status.rollSpCost;
		}
		m_rollInfo.rightGraceTime = grace_time;
	}
	
}

/// <summary>
/// 移動によるズーム
/// </summary>
/// <param name="camera">カメラ</param>
/// <param name="timer">ステップタイマー</param>
/// <param name="isBoosting">ブーストしているかどうか</param>
void MoveCommand::Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param::user_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.userParam;

	const float zoom_fov = parameter.zoomFov;
	const float zoom_time = parameter.zoomTime;

	if (isBoosting) {
		//画角を狭くする
		if (m_boostTime < zoom_time) {
			m_boostTime = std::min(m_boostTime + elapsed_time, zoom_time);
			float t = m_boostTime / zoom_time;
			t = t * (2 - t);
			float fov = m_defaultFov +  t * zoom_fov;
			camera.SetFov(fov);
		}
	}
	else {
		//画角を元に戻す
		if (m_boostTime > 0.0f) {
			m_boostTime = std::max(m_boostTime - elapsed_time, 0.0f);
			float t = m_boostTime / zoom_time;
			float fov = m_defaultFov + t * zoom_fov;
			camera.SetFov(fov);
		}
	}
}

/// <summary>
/// 照準によるカメラの位置を調整する
/// </summary>
/// <param name="targetCamera">ターゲットカメラ</param>
void MoveCommand::AdjustCamera(TargetCamera* targetCamera) {
	const CommandParameter::move_param::user_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.userParam;
	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	int width = ServiceLocater<DirectX11>::Get()->GetWidth();
	int height = ServiceLocater<DirectX11>::Get()->GetHeight();

	const float camera_rot_x_limit = parameter.cameraRotXLimit;
	const float camera_rot_y_limit = parameter.cameraRotYLimit;

	// 画面の中心から遠いほど回転量を大きくする
	DirectX::SimpleMath::Vector2 camera_rot(
		(mouse->GetPos().y - height / 2) / height * camera_rot_y_limit,
		-(mouse->GetPos().x - width / 2) / width * camera_rot_x_limit
	);

	DirectX::SimpleMath::Matrix camera_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(camera_rot.y, camera_rot.x, 0.0f);
	targetCamera->SetMatrix(camera_matrix);
}

