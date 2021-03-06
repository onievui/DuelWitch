#include "MoveCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Parameters\CommandParameter.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera/TargetCamera.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectID.h>
#include <Game\Effect\PlayerTrailEffectEmitter.h>
#include <Game\Effect\PlayerChargeEffectEmitter.h>


/// <summary>
/// コンストラクタ
/// </summary>
MoveCommand::MoveCommand()
	: m_state(MoveState::Move)
	, m_rollInfo()
	, m_turnInfo()
	, m_cameraTarget()
	, m_pTargetCamera(nullptr)
	, m_boostTime()
	, m_euler()
	, m_pTrailEffect()
	,m_pChargeEffect()
	, m_effectTransform(nullptr) {
}

/// <summary>
/// デストラクタ
/// </summary>
MoveCommand::~MoveCommand() {
	// カメラの追従を解除する
	m_pTargetCamera->SetTargetObject(nullptr);

	// エフェクトを消す
	m_pTrailEffect->SetUsed(false);
	m_pChargeEffect->SetUsed(false);
}

/// <summary>
/// 移動コマンドを初期化する
/// </summary>
/// <param name="player">プレイヤー</param>
void MoveCommand::Initialize(Player& player) {
	// カメラを取得する
	Camera& ref_camera = GetCamera(player);
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	if (target_camera) {
		m_pTargetCamera = target_camera;
		// カメラの追従先を設定する
		target_camera->SetTargetObject(&m_cameraTarget);
	}	

	// 初期の画角を記憶する
	m_defaultFov = m_pTargetCamera->GetFov();

	// 向きを初期化する
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

	// 前回の状態を初期化する
	m_prePos = GetTransform(player).GetPosition();
	m_preMove = DirectX::SimpleMath::Vector3::Zero;

	// ロールに関する情報を初期化する
	m_rollInfo.leftGraceTime = 0.0f;
	m_rollInfo.rightGraceTime = 0.0f;
	m_rollInfo.rollingTime = 0.0f;
	m_rollInfo.isRollingLeft = true;


	const EffectParameter::player_trail_param& parameter_t = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	Transform& transform = GetTransform(player);
	// エフェクト用の姿勢クラスの親オブジェクトにプレイヤーを登録する
	m_effectTransform.SetParent(&transform);
	m_effectTransform.SetPosition(parameter_t.appearPosOffset);

	// プレイヤーの軌跡エフェクトを生成する
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	// エフェクトの親オブジェクトにエフェクト用の姿勢クラスを登録する
	effect->SetParent(&m_effectTransform);
	m_pTrailEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pTrailEffect) {
		ErrorMessage(L"プレイヤーの軌跡エフェクトの生成に失敗しました");
	}

	// プレイヤーのチャージエフェクトを生成する
	const EffectParameter::player_charge_param& parameter_c = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerCharge, parameter_c.appearPosOffset, -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&transform);
	m_pChargeEffect = dynamic_cast<PlayerChargeEffectEmitter*>(effect);
	if (!m_pChargeEffect) {
		ErrorMessage(L"プレイヤーのチャージエフェクトの生成に失敗しました");
	}
}

/// <summary>
/// 移動コマンドを処理する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	const Transform& transform = GetTransform(player);
	// 補正後の前回位置
	DirectX::SimpleMath::Vector3 pre_pos = transform.GetPosition();

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
	// クイックターン
	case MoveCommand::MoveState::Turn:
		ExecuteTurn(player, timer);
		break;
	default:
		ErrorMessage(L"移動コマンドの状態が不正です");
		break;
	}

	// 移動量を保存する
	m_preMove = transform.GetPosition() - pre_pos;
	// 座標を保存する
	m_prePos = transform.GetPosition();

	// チャージエフェクトの状態を更新する
	const PlayerStatus& status = GetStatus(player);
	// チャージ中なら段階に応じて色を変える
	if (status.isCharging) {
		m_pChargeEffect->SetChargeState(static_cast<PlayerChargeEffectEmitter::State>(status.chargeLevel));
	}
	// チャージしていない場合は描画しない
	else {
		m_pChargeEffect->SetChargeState(PlayerChargeEffectEmitter::State::None);
	}

	// 照準のある方へカメラを少し向ける
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	if (target_camera) {
		AdjustCamera(target_camera);
		m_pTargetCamera = target_camera;
	}

}

/// <summary>
/// 通常移動の処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::ExcuteMove(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const InputManager* input_manager = ServiceLocater<InputManager>::Get();
	const DirectX::Keyboard::State key_state = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState();
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed = parameter.moveSpeed;
	const float rot_z_limit = parameter.rotZLimit;
	const float rot_x_limit = parameter.rotXLimit;
	const float lerp_speed = parameter.lerpSpeed;

	Transform& ref_transform = GetTransform(player);
	PlayerStatus& ref_status = GetStatus(player);

	// 衝突時に向きを調整する
	AdjustRotation(player, timer);

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();

	// 回転の変化量
	DirectX::SimpleMath::Vector3 change_euler;
	// ブーストしているかによって回転速度が変わる
	float rot_speed = (ref_status.isBoosting ? parameter.boostRotSpeed : parameter.rotSpeed);

	// 左右移動
	if (input_manager->IsDown(InputID::Left) || key_state.Left) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed * elapsed_time;
	}
	else if (input_manager->IsDown(InputID::Right) || key_state.Right) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed * elapsed_time;
	}
	//押していないときは戻す
	else {
		change_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed) - m_euler.z;
	}

	// 上下移動
	if (input_manager->IsDown(InputID::Up) || key_state.Up) {
		change_euler.x = -rot_speed * elapsed_time;
	}
	else if (input_manager->IsDown(InputID::Down) || key_state.Down) {
		change_euler.x = rot_speed * elapsed_time;
	}

	// 斜め移動の場合は回転量を調整する
	if (!Math::Equal0(change_euler.x) && !Math::Equal0(change_euler.y)) {
		change_euler /= std::sqrtf(2);
	}

	// 回転を計算する
	m_euler += change_euler;
	m_euler.x = Math::Clamp(m_euler.x, -rot_x_limit, rot_x_limit);
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);

	// 移動方向を計算する
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	
	// ブーストボタンを押していて、残りSPが10％以上ならブースト移動
	const bool use_boost = input_manager->IsDown(InputID::Boost);
	const bool can_boost = ref_status.sp / ref_status.maxSp >= 0.1f;
	if (use_boost && can_boost) {
		move *= move_speed * elapsed_time*ref_status.boostSpeedRate;
		pos += move;
		// SPを減らす
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		// ブースト開始直後なら効果音を鳴らす
		if (!ref_status.isBoosting) {
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Boost);
		}
		ref_status.isBoosting = true;
	}
	// 通常移動
	else {
		move *= move_speed * elapsed_time;
		pos += move;
		ref_status.isBoosting = false;
	}

	// 移動量の保存
	m_preMove = move;

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
	// クイックターンの入力判定を行う
	TurnInputCheck(player, timer);
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
	// 移動量
	DirectX::SimpleMath::Vector3 roll_move(0, 0, 0);

	// 移動後の進行度
	float t = m_rollInfo.rollingTime / rolling_time;
	t = t * (2 - t);
	// 移動前の進行度
	float t2 = (m_rollInfo.rollingTime - elapsed_time) / rolling_time;
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
/// クイックターンの処理を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::ExecuteTurn(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed = parameter.moveSpeed;
	const float quick_turn_time = parameter.quickTurnTime;

	PlayerStatus& ref_status = GetStatus(player);
	Transform& ref_transform = GetTransform(player);
	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	m_turnInfo.turningTime += elapsed_time;
	// 移動量
	DirectX::SimpleMath::Vector3 turn_move(0, 0, 0);
	// 移動速度倍率
	float move_speed_rate = 1.0f;

	// 移動後の進行度
	float t = m_turnInfo.turningTime / quick_turn_time;
	t = Math::Clamp(t, 0.0f, 1.0f);
	t = t * (2 - t);

	// 回転させる
	m_euler.x = m_turnInfo.preRotX * (t*-2 + 1);
	m_euler.y = m_turnInfo.preRotY + Math::PI*t*m_turnInfo.turnDirection;
	// 移動速度倍率を求める
	move_speed_rate = (t < 0.5f ? 1-(t * 2)*(2 - t * 2) : std::powf((t - 0.5f)*2, 2));
	
	// 回転量と移動量を計算する
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	pos += move * move_speed*move_speed_rate*elapsed_time;

	// カメラのズームを制御する
	ref_status.isBoosting = false;
	Zoom(GetCamera(player), timer, false);

	// 回転と移動を反映する
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// カメラの追従先を更新する
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.GetTransformRef().SetRotation(rotation);
	m_cameraTarget.Update(timer);

	// クイックターンが終わったら元の状態に戻す
	if (m_turnInfo.turningTime > quick_turn_time) {
		// 入力をリセットする
		m_turnInfo.graceTime = 0.0f;

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
	const InputManager* input_manager = ServiceLocater<InputManager>::Get();
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
	const bool press_left = (input_manager->IsPressed(InputID::LeftRoll) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Left));
	if (press_left) {
		// パッド未接続時は素早く2回入力した時に判定する
		if (m_rollInfo.leftGraceTime > 0.0f || input_manager->IsPadConnected()) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = true;
			m_rollInfo.preRotZ = m_euler.z;
			// SPを消費する
			ref_status.sp -= ref_status.rollSpCost;
			// 効果音を鳴らす
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_rollInfo.leftGraceTime = grace_time;
	}
	
	// 右にロールする
	const bool press_right = (input_manager->IsPressed(InputID::RightRoll) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Right));
	if (press_right) {
		// パッド未接続時は素早く2回入力した時に判定する
		if (m_rollInfo.rightGraceTime > 0.0f || input_manager->IsPadConnected()) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = false;
			m_rollInfo.preRotZ = m_euler.z;
			// SPを消費する
			ref_status.sp -= ref_status.rollSpCost;
			// 効果音を鳴らす
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_rollInfo.rightGraceTime = grace_time;
	}
	
}

/// <summary>
/// クイックターンの入力判定を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::TurnInputCheck(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const InputManager* input_manager = ServiceLocater<InputManager>::Get();
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	PlayerStatus& ref_status = GetStatus(player);

	// タイマーを進める
	m_turnInfo.graceTime -= elapsed_time;

	// SPが足りない時は判定しない
	if (ref_status.sp < ref_status.quickTurnSpCost) {
		return;
	}

	// クイックターンの入力判定を行う
	const float grace_time = 0.2f;
	const bool press_down = (input_manager->IsPressed(InputID::Turn) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Down));
	const bool down_left = (input_manager->IsDown(InputID::Left) || key_tracker->GetLastState().Left);
	const bool down_right = (input_manager->IsDown(InputID::Right) || key_tracker->GetLastState().Right);
	if (press_down) {
		// パッド未接続時は素早く2回入力した時に判定する
		if (m_turnInfo.graceTime > 0.0f || input_manager->IsPadConnected()) {
			m_state = MoveState::Turn;
			m_turnInfo.turningTime = 0.0f;
			// 回転しているならその方向へ回る
			if (!down_left && down_right) {
				m_turnInfo.turnDirection = -1;
			}
			else {
				m_turnInfo.turnDirection = 1;
			}
			m_turnInfo.preRotX = m_euler.x;
			m_turnInfo.preRotY = m_euler.y;
			// SPを消費する
			ref_status.sp -= ref_status.quickTurnSpCost;
			// 効果音を鳴らす
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_turnInfo.graceTime = grace_time;
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

/// <summary>
/// 衝突時に向きを調整する
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void MoveCommand::AdjustRotation(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	Transform& ref_transform = GetTransform(player);

	// 変化がないなら何もしない
	if (DirectX::SimpleMath::Vector3::DistanceSquared(m_prePos, ref_transform.GetPosition()) < 0.01f) {
		return;
	}

	// 本来の移動方向
	DirectX::SimpleMath::Vector3 old_vec = m_preMove;
	// 現在の移動方向
	DirectX::SimpleMath::Vector3 new_vec = ref_transform.GetPosition() - (m_prePos - m_preMove);
	// Y方向の移動は無視する
	old_vec.y = 0;
	if (old_vec.LengthSquared() <= Math::Epsilon) {
		return;
	}
	old_vec.Normalize();
	new_vec.y = 0;
	if (new_vec.LengthSquared() <= Math::Epsilon) {
		return;
	}
	new_vec.Normalize();
	// 角度から回転量を求める
	float angle = Math::BetweenAngle(old_vec, new_vec);
	float power = angle*elapsed_time;
	m_euler.y += power*(old_vec.Cross(new_vec).y > 0 ? 1 : -1);
}

