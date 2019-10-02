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
/// �R���X�g���N�^
/// </summary>
MoveCommand::MoveCommand()
	: m_cameraTarget()
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
	//TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&ref_camera);
	//// �^�[�Q�b�g�J�����łȂ��ꍇ�͏��������Ȃ�
	//if (!target_camera) {
	//	return;
	//}
	//// �Ǐ]����I�u�W�F�N�g�����݂��Ȃ��ꍇ�̓^�[�Q�b�g��ݒ肷��
	//if (!target_camera->HasTargetObject()) {
	//	target_camera->SetTargetObject(&m_cameraTarget);
	//}

	// �����̉�p���L������
	m_defaultFov = ref_camera.GetFov();

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

	DirectX::SimpleMath::Vector3 pos = ref_transform.GetLocalPosition();
	DirectX::SimpleMath::Vector3 move(0, 0, 0);

	const float rot_speed = Math::PI / 3;
	const float rot_x_lim = Math::PI / 3;

	// ��]�̕ω���
	DirectX::SimpleMath::Vector3 change_euler;

	// ���E�ړ�
	if (key_state.A || key_state.Left) {
		change_euler.z = Math::Lerp(m_euler.z, -rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = rot_speed *elapsed_time;
	}
	else if (key_state.D || key_state.Right) {
		change_euler.z = Math::Lerp(m_euler.z, rot_z_limit, lerp_speed) - m_euler.z;
		change_euler.y = -rot_speed *elapsed_time;
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
	if (change_euler.x != 0.0f && change_euler.y != 0.0f) {
		change_euler *= 0.5f;
	}

	m_euler += change_euler;

	m_euler.x = Math::Clamp(m_euler.x, -rot_x_lim, rot_x_lim);

	move = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ,
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));

	MouseWrapper* mouse = ServiceLocater<MouseWrapper>::Get();
	Player::Status& ref_status = GetStatus(player);
	// �E�N���b�N���Ă��āA�c��SP��10���ȏ�Ȃ�u�[�X�g�ړ�
	if (mouse->GetTracker()->rightButton == DirectX::Mouse::ButtonStateTracker::ButtonState::HELD &&
		ref_status.sp / ref_status.maxSp >= 0.1f) {
		pos += move * move_speed*elapsed_time*ref_status.boostSpeedRate;
		// SP�����炷
		ref_status.sp -= ref_status.boostSpCost*elapsed_time;
		ref_status.isBoosting = true;
		Zoom(GetCamera(player), timer, true);
	}
	else {
		pos += move * move_speed*elapsed_time;
		ref_status.isBoosting = false;
		Zoom(GetCamera(player), timer, false);
	}

	//if (pos.Length() > 80.0f) {
	//	pos.Normalize();
	//	pos = pos * 80.0f;
	//}

	ref_transform.SetPosition(pos);
	ref_transform.SetRotation(DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_euler.y, m_euler.x, m_euler.z));
	GetWorld(player) = ref_transform.GetMatrix();

	// �Ə��̂�����փJ����������������
	int width = ServiceLocater<DirectX11>::Get()->GetWidth();
	int height = ServiceLocater<DirectX11>::Get()->GetHeight();
	const float camera_rot_x_limit = parameter.cameraRotXLimit;
	const float camera_rot_y_limit = parameter.cameraRotYLimit;
	DirectX::SimpleMath::Vector2 camera_rot(
		(mouse->GetPos().y - height / 2) / height * camera_rot_y_limit,
		-(mouse->GetPos().x - width / 2) / width * camera_rot_x_limit
	);
	DirectX::SimpleMath::Matrix camera_matrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(camera_rot.y, camera_rot.x, 0.0f);
	TargetCamera* target_camera = dynamic_cast<TargetCamera*>(&GetCamera(player));
	if (target_camera) {
		target_camera->SetMatrix(camera_matrix);
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
	const float zoom_fov = Math::PI / 18;
	const float zoom_time = 0.5f;

	if (isBoosting) {
		//��p����������
		if (m_boostTime < zoom_time) {
			m_boostTime = std::min(m_boostTime + elapsed_time, zoom_time);
			float fov = m_defaultFov + m_boostTime / zoom_time * zoom_fov;
			camera.SetFov(fov);
		}
	}
	else {
		//��p�����ɖ߂�
		if (m_boostTime > 0.0f) {
			m_boostTime = std::max(m_boostTime - elapsed_time, 0.0f);
			float fov = m_defaultFov + m_boostTime / zoom_time * zoom_fov;
			camera.SetFov(fov);
		}
	}
}

