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
/// �R���X�g���N�^
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
/// �ړ��R�}���h������������
/// </summary>
/// <param name="player">�v���C���[</param>
void MoveCommand::Initialize(Player& player) {
	Camera& ref_camera = GetCamera(player);

	// �J�����̒Ǐ]���ݒ肷��
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	if (target_camera) {
		target_camera->SetTargetObject(&m_cameraTarget);
	}

	// �����̉�p���L������
	m_defaultFov = ref_camera.GetFov();

	// ����������������
	DirectX::SimpleMath::Matrix rot_matrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(GetTransform(player).GetRotation());
	m_euler.y = std::atan2f(-rot_matrix._31, rot_matrix._33);

	// ���[���Ɋւ����������������
	m_rollInfo.leftGraceTime = 0.0f;
	m_rollInfo.rightGraceTime = 0.0f;
	m_rollInfo.rollingTime = 0.0f;
	m_rollInfo.isRollingLeft = true;

	// �v���C���[�̋O�ՃG�t�F�N�g�𐶐�����
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	m_effectTransform.SetParent(&GetTransform(player));
	m_effectTransform.SetPosition(parameter.appearPosOffset);
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(
		EffectID::PlayerTrail, m_effectTransform.GetPosition(), -DirectX::SimpleMath::Vector3::UnitZ);
	effect->SetParent(&m_effectTransform);
	m_pEffect = dynamic_cast<PlayerTrailEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"�v���C���[�̋O�ՃG�t�F�N�g�̐����Ɏ��s���܂���");
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
	default:
		ErrorMessage(L"�ړ��R�}���h�̏�Ԃ��s���ł�");
		break;
	}

	// �Ə��̂�����փJ����������������
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	if (target_camera) {
		AdjustCamera(target_camera);
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

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();

	// ��]�̕ω���
	DirectX::SimpleMath::Vector3 change_euler;
	// ��]��
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

	// �΂߈ړ��̏ꍇ
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
	if (mouse->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::ButtonState::HELD &&
		ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SP�����炷
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
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
	DirectX::SimpleMath::Vector3 roll_move(0, 0, 0);

	float t = m_rollInfo.rollingTime / rolling_time;
	float t2 = (m_rollInfo.rollingTime - elapsed_time) / rolling_time;
	t = t * (2 - t);
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
	if (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::A) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Left)) {
		// �f����2����͂������ɔ��肷��
		if (m_rollInfo.leftGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = true;
			m_rollInfo.preRotZ = m_euler.z;
			// SP�������
			ref_status.sp -= ref_status.rollSpCost;
		}
		m_rollInfo.leftGraceTime = grace_time;
	}
	
	// �E�Ƀ��[������
	if (key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::D) || key_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Right)) {
		// �f����2����͂������ɔ��肷��
		if (m_rollInfo.rightGraceTime > 0.0f) {
			m_state = MoveState::Roll;
			m_rollInfo.rollingTime = 0.0f;
			m_rollInfo.isRollingLeft = false;
			m_rollInfo.preRotZ = m_euler.z;
			// SP�������
			ref_status.sp -= ref_status.rollSpCost;
		}
		m_rollInfo.rightGraceTime = grace_time;
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

