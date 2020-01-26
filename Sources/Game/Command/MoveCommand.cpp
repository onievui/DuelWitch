#include "MoveCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
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
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
MoveCommand::~MoveCommand() {
	// �J�����̒Ǐ]����������
	m_pTargetCamera->SetTargetObject(nullptr);

	// �G�t�F�N�g������
	m_pTrailEffect->SetUsed(false);
	m_pChargeEffect->SetUsed(false);
}

/// <summary>
/// �ړ��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void MoveCommand::Initialize(Player& player) {
	// �J�������擾����
	Camera& ref_camera = GetCamera(player);
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	if (target_camera) {
		m_pTargetCamera = target_camera;
		// �J�����̒Ǐ]���ݒ肷��
		target_camera->SetTargetObject(&m_cameraTarget);
	}	

	// �����̉�p���L������
	m_defaultFov = m_pTargetCamera->GetFov();

	// ����������������
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

	// ���[���Ɋւ����������������
	m_rollInfo.leftGraceTime = 0.0f;
	m_rollInfo.rightGraceTime = 0.0f;
	m_rollInfo.rollingTime = 0.0f;
	m_rollInfo.isRollingLeft = true;


	const EffectParameter::player_trail_param& parameter_t = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	Transform& transform = GetTransform(player);
	// �G�t�F�N�g�p�̎p���N���X�̐e�I�u�W�F�N�g�Ƀv���C���[��o�^����
	m_effectTransform.SetParent(&transform);
	m_effectTransform.SetPosition(parameter_t.appearPosOffset);

	// �v���C���[�̋O�ՃG�t�F�N�g�𐶐�����
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	// �G�t�F�N�g�̐e�I�u�W�F�N�g�ɃG�t�F�N�g�p�̎p���N���X��o�^����
	effect->SetParent(&m_effectTransform);
	m_pTrailEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pTrailEffect) {
		ErrorMessage(L"�v���C���[�̋O�ՃG�t�F�N�g�̐����Ɏ��s���܂���");
	}

	// �v���C���[�̃`���[�W�G�t�F�N�g�𐶐�����
	const EffectParameter::player_charge_param& parameter_c = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerCharge, parameter_c.appearPosOffset, -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&transform);
	m_pChargeEffect = dynamic_cast<PlayerChargeEffectEmitter*>(effect);
	if (!m_pChargeEffect) {
		ErrorMessage(L"�v���C���[�̃`���[�W�G�t�F�N�g�̐����Ɏ��s���܂���");
	}
}

/// <summary>
/// �ړ��R�}���h����������
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void MoveCommand::Execute(Player& player, const DX::StepTimer& timer) {
	// ��Ԃɂ���ď����𕪂���
	switch (m_state) {
	// �ʏ�ړ�
	case MoveCommand::MoveState::Move:
		ExcuteMove(player, timer);
		break;
	// ���[�����
	case MoveCommand::MoveState::Roll:
		ExcuteRoll(player, timer);
		break;
	// �N�C�b�N�^�[��
	case MoveCommand::MoveState::Turn:
		ExecuteTurn(player, timer);
		break;
	default:
		ErrorMessage(L"�ړ��R�}���h�̏�Ԃ��s���ł�");
		break;
	}

	// �`���[�W�G�t�F�N�g�̏�Ԃ��X�V����
	const PlayerStatus& status = GetStatus(player);
	// �`���[�W���Ȃ�i�K�ɉ����ĐF��ς���
	if (status.isCharging) {
		m_pChargeEffect->SetChargeState(static_cast<PlayerChargeEffectEmitter::State>(status.chargeLevel));
	}
	// �`���[�W���Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
	else {
		m_pChargeEffect->SetChargeState(PlayerChargeEffectEmitter::State::None);
	}

	// �Ə��̂�����փJ����������������
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	if (target_camera) {
		AdjustCamera(target_camera);
		m_pTargetCamera = target_camera;
	}

}

/// <summary>
/// �ʏ�ړ��̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
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

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetPosition();

	// ��]�̕ω���
	DirectX::SimpleMath::Vector3 change_euler;
	// �u�[�X�g���Ă��邩�ɂ���ĉ�]���x���ς��
	float rot_speed = (ref_status.isBoosting ? parameter.boostRotSpeed : parameter.rotSpeed);

	// ���E�ړ�
	if (key_state.A || key_state.Left) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed * elapsed_time;
	}
	else if (key_state.D || key_state.Right) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed * elapsed_time;
	}
	//�����Ă��Ȃ��Ƃ��͖߂�
	else {
		change_euler.z = Math::Lerp(m_euler.z, 0.0f, lerp_speed) - m_euler.z;
	}

	// �㉺�ړ�
	if (key_state.W || key_state.Up) {
		change_euler.x = -rot_speed * elapsed_time;
	}
	else if (key_state.S || key_state.Down) {
		change_euler.x = rot_speed * elapsed_time;
	}

	// �΂߈ړ��̏ꍇ�͉�]�ʂ𒲐�����
	if (!Math::Equal0(change_euler.x) && !Math::Equal0(change_euler.y)) {
		change_euler /= std::sqrtf(2);
	}

	// ��]���v�Z����
	m_euler += change_euler;
	m_euler.x = Math::Clamp(m_euler.x, -rot_x_limit, rot_x_limit);
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);

	// �ړ��������v�Z����
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	
	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	// �E�N���b�N���Ă��āA�c��SP��10���ȏ�Ȃ�u�[�X�g�ړ�
	const bool use_boost = mouse->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::ButtonState::HELD;
	const bool can_boost = ref_status.sp / ref_status.maxSp >= 0.1f;
	if (use_boost && can_boost) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SP�����炷
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		// �u�[�X�g�J�n����Ȃ���ʉ���炷
		if (!ref_status.isBoosting) {
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Boost);
		}
		ref_status.isBoosting = true;
	}
	// �ʏ�ړ�
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
	}

	// �J�����̃Y�[���𐧌䂷��
	Zoom(GetCamera(player), timer, ref_status.isBoosting);

	// �ړ��Ɖ�]�𔽉f���ă��[���h�s������߂Ă���
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// �J�����̒Ǐ]����X�V����
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.GetTransformRef().SetRotation(rotation);
	m_cameraTarget.Update(timer);

	// ���[������̓��͔�����s��
	RollInputCheck(player, timer);
	// �N�C�b�N�^�[���̓��͔�����s��
	TurnInputCheck(player, timer);
}

/// <summary>
/// ���[������̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
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
	// �ړ���
	DirectX::SimpleMath::Vector3 roll_move(0, 0, 0);

	// �ړ���̐i�s�x
	float t = m_rollInfo.rollingTime / rolling_time;
	t = t * (2 - t);
	// �ړ��O�̐i�s�x
	float t2 = (m_rollInfo.rollingTime - elapsed_time) / rolling_time;
	t2 = t2 * (2 - t2);
	// ���Ƀ��[������ꍇ
	if (m_rollInfo.isRollingLeft) {
		// ��]������
		m_euler.z = m_rollInfo.preRotZ - roll_angle * t;
		if (m_euler.z < -Math::PI) {
			m_euler.z += Math::PI2;
		}
		// �ړ��ʂ����߂�
		roll_move = DirectX::SimpleMath::Vector3::Right * (t - t2)*roll_move_length;
	}
	// �E�Ƀ��[������ꍇ
	else {
		// ��]������
		m_euler.z = m_rollInfo.preRotZ + roll_angle * t;
		if (m_euler.z > Math::PI) {
			m_euler.z -= Math::PI2;
		}
		// �ړ��ʂ����߂�
		roll_move = DirectX::SimpleMath::Vector3::Left * (t - t2)*roll_move_length;
	}

	// ��]�ʂƈړ��ʂ��v�Z����
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);
	DirectX::SimpleMath::Quaternion rotation_xy = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, 0);
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	roll_move = DirectX::SimpleMath::Vector3::Transform(roll_move, rotation_xy);
	pos += move * move_speed*elapsed_time + roll_move;

	// �J�����̃Y�[���𐧌䂷��
	ref_status.isBoosting = false;
	Zoom(GetCamera(player), timer, false);

	// ��]�ƈړ��𔽉f����
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// �J�����̒Ǐ]����X�V����
	// ��]�͕ύX���Ȃ�
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.Update(timer);

	// ��]���I������猳�̏�Ԃɖ߂�
	if (m_rollInfo.rollingTime > rolling_time) {
		// ���͂����Z�b�g����
		m_rollInfo.leftGraceTime = 0.0f;
		m_rollInfo.rightGraceTime = 0.0f;

		m_state = MoveState::Move;
	}
}

/// <summary>
/// �N�C�b�N�^�[���̏������s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void MoveCommand::ExecuteTurn(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam;

	const float move_speed = parameter.moveSpeed;
	const float quick_turn_time = parameter.quickTurnTime;

	PlayerStatus& ref_status = GetStatus(player);
	Transform& ref_transform = GetTransform(player);
	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	m_turnInfo.turningTime += elapsed_time;
	// �ړ���
	DirectX::SimpleMath::Vector3 turn_move(0, 0, 0);
	// �ړ����x�{��
	float move_speed_rate = 1.0f;

	// �ړ���̐i�s�x
	float t = m_turnInfo.turningTime / quick_turn_time;
	t = Math::Clamp(t, 0.0f, 1.0f);
	t = t * (2 - t);

	// ��]������
	m_euler.x = m_turnInfo.preRotX * (t*-2 + 1);
	m_euler.y = m_turnInfo.preRotY + Math::PI*t*m_turnInfo.turnDirection;
	// �ړ����x�{�������߂�
	move_speed_rate = (t < 0.5f ? 1-(t * 2)*(2 - t * 2) : std::powf((t - 0.5f)*2, 2));
	
	// ��]�ʂƈړ��ʂ��v�Z����
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z);
	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, rotation);
	pos += move * move_speed*move_speed_rate*elapsed_time;

	// �J�����̃Y�[���𐧌䂷��
	ref_status.isBoosting = false;
	Zoom(GetCamera(player), timer, false);

	// ��]�ƈړ��𔽉f����
	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(rotation);
	GetWorld(player) = ref_transform.GetMatrix();

	// �J�����̒Ǐ]����X�V����
	m_cameraTarget.GetTransformRef().SetPosition(pos);
	m_cameraTarget.GetTransformRef().SetRotation(rotation);
	m_cameraTarget.Update(timer);

	// �N�C�b�N�^�[�����I������猳�̏�Ԃɖ߂�
	if (m_turnInfo.turningTime > quick_turn_time) {
		// ���͂����Z�b�g����
		m_turnInfo.graceTime = 0.0f;

		m_state = MoveState::Move;
	}
}

/// <summary>
/// ���[������̓��͔�����s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void MoveCommand::RollInputCheck(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	PlayerStatus& ref_status = GetStatus(player);

	// �^�C�}�[��i�߂�
	m_rollInfo.leftGraceTime -= elapsed_time;
	m_rollInfo.rightGraceTime -= elapsed_time;

	// SP������Ȃ����͔��肵�Ȃ�
	if (ref_status.sp < ref_status.rollSpCost) {
		return;
	}

	// ���[������̓��͔�����s��
	const float grace_time = 0.2f;
	// ���Ƀ��[������
	const bool press_left = (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::A) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Left));
	if (press_left) {
		// �f����2����͂������ɔ��肷��
		if (m_rollInfo.leftGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = true;
			m_rollInfo.preRotZ = m_euler.z;
			// SP�������
			ref_status.sp -= ref_status.rollSpCost;
			// ���ʉ���炷
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_rollInfo.leftGraceTime = grace_time;
	}
	
	// �E�Ƀ��[������
	const bool press_right = (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::D) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Right));
	if (press_right) {
		// �f����2����͂������ɔ��肷��
		if (m_rollInfo.rightGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = false;
			m_rollInfo.preRotZ = m_euler.z;
			// SP�������
			ref_status.sp -= ref_status.rollSpCost;
			// ���ʉ���炷
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_rollInfo.rightGraceTime = grace_time;
	}
	
}

/// <summary>
/// �N�C�b�N�^�[���̓��͔�����s��
/// </summary>
/// <param name="player">�v���C���[</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void MoveCommand::TurnInputCheck(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	DirectX::Keyboard::KeyboardStateTracker* key_tracker = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get();
	PlayerStatus& ref_status = GetStatus(player);

	// �^�C�}�[��i�߂�
	m_turnInfo.graceTime -= elapsed_time;

	// SP������Ȃ����͔��肵�Ȃ�
	if (ref_status.sp < ref_status.quickTurnSpCost) {
		return;
	}

	// �N�C�b�N�^�[���̓��͔�����s��
	const float grace_time = 0.2f;
	const bool press_down = (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::S) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Down));
	const bool down_left = (key_tracker->GetLastState().A || key_tracker->GetLastState().Left);
	const bool down_right = (key_tracker->GetLastState().D || key_tracker->GetLastState().Right);
	if (press_down) {
		// �f����2����͂������ɔ��肷��
		if (m_turnInfo.graceTime > 0.0f) {
			m_state = MoveState::Turn;
			m_turnInfo.turningTime = 0.0f;
			// ��]���Ă���Ȃ炻�̕����։��
			if (!down_left && down_right) {
				m_turnInfo.turnDirection = -1;
			}
			else {
				m_turnInfo.turnDirection = 1;
			}
			m_turnInfo.preRotX = m_euler.x;
			m_turnInfo.preRotY = m_euler.y;
			// SP�������
			ref_status.sp -= ref_status.quickTurnSpCost;
			// ���ʉ���炷
			ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Rolling);
		}
		m_turnInfo.graceTime = grace_time;
	}
}

/// <summary>
/// �ړ��ɂ��Y�[��
/// </summary>
/// <param name="camera">�J����</param>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="isBoosting">�u�[�X�g���Ă��邩�ǂ���</param>
void MoveCommand::Zoom(Camera& camera, const DX::StepTimer& timer, bool isBoosting) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const CommandParameter::move_param::user_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.userParam;

	const float zoom_fov = parameter.zoomFov;
	const float zoom_time = parameter.zoomTime;

	if (isBoosting) {
		//��p����������
		if (m_boostTime < zoom_time) {
			m_boostTime = std::min(m_boostTime + elapsed_time, zoom_time);
			float t = m_boostTime / zoom_time;
			t = t * (2 - t);
			float fov = m_defaultFov +  t * zoom_fov;
			camera.SetFov(fov);
		}
	}
	else {
		//��p�����ɖ߂�
		if (m_boostTime > 0.0f) {
			m_boostTime = std::max(m_boostTime - elapsed_time, 0.0f);
			float t = m_boostTime / zoom_time;
			float fov = m_defaultFov + t * zoom_fov;
			camera.SetFov(fov);
		}
	}
}

/// <summary>
/// �Ə��ɂ��J�����̈ʒu�𒲐�����
/// </summary>
/// <param name="targetCamera">�^�[�Q�b�g�J����</param>
void MoveCommand::AdjustCamera(TargetCamera* targetCamera) {
	const CommandParameter::move_param::user_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetCommandParameter()->moveParam.userParam;
	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	int width = ServiceLocater<DirectX11>::Get()->GetWidth();
	int height = ServiceLocater<DirectX11>::Get()->GetHeight();

	const float camera_rot_x_limit = parameter.cameraRotXLimit;
	const float camera_rot_y_limit = parameter.cameraRotYLimit;

	// ��ʂ̒��S���牓���قǉ�]�ʂ�傫������
	DirectX::SimpleMath::Vector2 camera_rot(
		(mouse->GetPos().y - height / 2) / height * camera_rot_y_limit,
		-(mouse->GetPos().x - width / 2) / width * camera_rot_x_limit
	);

	DirectX::SimpleMath::Matrix camera_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(camera_rot.y, camera_rot.x, 0.0f);
	targetCamera->SetMatrix(camera_matrix);
}

