#include "MouseWrapper.h"
#include <Utils\MathUtils.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="hWnd">�E�C���h�E�n���h��</param>
MouseWrapper::MouseWrapper(HWND hWnd)
	: m_sensivity(1.0f)
	, m_pos()
	, m_moved() {
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(hWnd);
	m_tracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MouseWrapper::~MouseWrapper() {
	m_mouse.reset();
	m_tracker.reset();
}

/// <summary>
/// �}�E�X���X�V����
/// </summary>
/// <param name="width">�E�C���h�E��</param>
/// <param name="height">�E�C���h�E��</param>
void MouseWrapper::Update(int width, int height) {
	DirectX::Mouse::State state = m_mouse->GetState();
	m_tracker->Update(state);

	// �}�E�X�̃��[�h����Βl�Q�ƂȂ�
	if (state.positionMode == DirectX::Mouse::Mode::MODE_ABSOLUTE) {
		m_moved.x = state.x - m_pos.x;
		m_moved.y = state.y - m_pos.y;
		m_pos.x = static_cast<float>(state.x);
		m_pos.y = static_cast<float>(state.y);
	}
	// �}�E�X�̃��[�h�����Βl�Q�ƂȂ�
	else {
		m_moved.x = state.x*m_sensivity;
		m_moved.y = state.y*m_sensivity;
		// �}�E�X�̈ʒu���E�C���h�E���Ɏ��߂�
		m_pos = Math::Clamp(m_pos + m_moved, DirectX::SimpleMath::Vector2::Zero,
			DirectX::SimpleMath::Vector2(static_cast<float>(width), static_cast<float>(height)));
	}
}

/// <summary>
/// �}�E�X�̏�Ԃ����Z�b�g����
/// </summary>
void MouseWrapper::Reset() {
	m_tracker->Reset();
}

/// <summary>
/// �}�E�X���擾����
/// </summary>
/// <returns>
/// �}�E�X�ւ̃|�C���^
/// </returns>
DirectX::Mouse* MouseWrapper::GetMouse() {
	return m_mouse.get();
}

/// <summary>
/// �g���b�J�[���擾����
/// </summary>
/// <returns>
/// �g���b�J�[�ւ̃|�C���^
/// </returns>
DirectX::Mouse::ButtonStateTracker* MouseWrapper::GetTracker() {
	return m_tracker.get();
}

/// <summary>
/// �}�E�X���x��ݒ肷��
/// </summary>
/// <param name="sensivity">���x</param>
void MouseWrapper::SetSensivity(float sensivity) {
	m_sensivity = sensivity;
}

/// <summary>
/// �}�E�X���x���擾����
/// </summary>
/// <returns>
/// ���x
/// </returns>
float MouseWrapper::GetSensivity() const {
	return m_sensivity;
}

/// <summary>
/// �}�E�X�̍��W���擾����
/// </summary>
/// <returns>
/// �}�E�X�̍��W
/// </returns>
const DirectX::SimpleMath::Vector2& MouseWrapper::GetPos() const {
	return m_pos;
}

/// <summary>
/// �}�E�X�̈ړ��ʂ��擾����
/// </summary>
/// <returns>
/// �}�E�X�̈ړ���
/// </returns>
const DirectX::SimpleMath::Vector2& MouseWrapper::GetMoved() const {
	return m_moved;
}

/// <summary>
/// �}�E�X�̍��W���[�h��ݒ肷��
/// </summary>
/// <param name="mode">�ݒ肷�郂�[�h</param>
void MouseWrapper::SetMode(DirectX::Mouse::Mode mode) {
	m_mouse->SetMode(mode);
}
