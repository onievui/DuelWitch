#include "DebugCamera.h"


const float DebugCamera::CAMERA_DISTANCE = 5.0f;


// �R���X�g���N�^
DebugCamera::DebugCamera(int width, int height)
	: m_yAngle(0.0f)
	, m_xAngle(0.0f)
	, m_xTmp(0.0f)
	, m_yTmp(0.0f)
	, m_x(0)
	, m_y(0)
	, m_scrollWheelValue(0) {
	AdjustWindowScale(width, height);
}

// �X�V����
void DebugCamera::Update()
{
	// ���_�x�N�g��
	DirectX::SimpleMath::Vector3 eye(0.0f, 0.0f, 1.0f);
	// �����_�x�N�g��
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	// �}�E�X�̏�Ԃ��擾����
	DirectX::Mouse::State state = DirectX::Mouse::Get().GetState();

	// ���΃��[�h�̏ꍇ�ɖ߂�
	if (state.positionMode == DirectX::Mouse::MODE_RELATIVE)
		return;

	// �g���b�L���O���X�V����
	m_tracker.Update(state);

	// �}�E�X�̍��{�^���������ꂽ�ꍇ
	if (m_tracker.leftButton == DirectX::Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// �}�E�X�̍��W���擾����
		m_x = state.x;
		m_y = state.y;
	}
	// �}�E�X�̍��{�^����������ꂽ�ꍇ
	else if (m_tracker.leftButton == DirectX::Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// ���݂̉�]��ۑ�����
		m_xAngle = m_xTmp;
		m_yAngle = m_yTmp;
	}
	// �}�E�X�̃{�^����������Ă�����J�������ړ�������
	if (state.leftButton)
	{
		Motion(state.x, state.y);
	}

	// �}�E�X�̃z�C�[���l���擾����
	m_scrollWheelValue = state.scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		// �z�C�[���l������������
		m_scrollWheelValue = 0;
		// �X�N���[���z�C�[���l�����Z�b�g����
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// �r���[�s����v�Z����
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_yTmp);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_xTmp);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;



	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (CAMERA_DISTANCE - m_scrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());
	// ���_���X�V����
	m_eye = eye;
	// �����_���X�V����
	m_target = target;
	// �r���[���X�V����
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}

// �s��𐶐�����
void DebugCamera::Motion(int x, int y)
{
	// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ�(���Βl)���v�Z����
	float dx = (x - m_x) * m_xScale;
	float dy = (y - m_y) * m_yScale;

	if (dx != 0.0f || dy != 0.0f)
	{
		// �x���̉�]���v�Z����
		float yAngle = dx * DirectX::XM_PI;
		// �w���̉�]���v�Z����
		float xAngle = dy * DirectX::XM_PI;

		m_xTmp = m_xAngle + xAngle;
		m_yTmp = m_yAngle + yAngle;
	}
}

// �f�o�b�O�J�����s����擾����
DirectX::SimpleMath::Matrix DebugCamera::GetCameraMatrix() const
{
	return m_view;
}

// ���_���擾����
DirectX::SimpleMath::Vector3 DebugCamera::GetEyePosition() const
{
	return m_eye;
}

// �����_���擾����
DirectX::SimpleMath::Vector3 DebugCamera::GetTargetPosition() const
{
	return m_target;
}

void DebugCamera::AdjustWindowScale(int width, int height)
{
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���𒲐�����
	m_xScale = 1.0f / float(width);
	m_yScale = 1.0f / float(height);
}
