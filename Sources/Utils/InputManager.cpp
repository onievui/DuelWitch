#include "InputManager.h"
#include <Utils\ErrorMessage.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="keyCode">�Ή�����L�[�R�[�h</param>
/// <param name="padCode">�Ή�����p�b�h�R�[�h</param>
/// <param name="mouseCode">�Ή�����}�E�X�R�[�h</param>
InputManager::InputUnit::InputUnit(KeyCode keyCode, PadCode padCode, MouseCode mouseCode) 
	: m_keyCode(keyCode)
	, m_padCode(padCode)
	, m_mouseCode(mouseCode) {
}

/// <summary>
/// ���͂��Ă��邩�擾����
/// </summary>
/// <returns>
/// true : ���͂��Ă���
/// false : ���͂��Ă��Ȃ�
/// </returns>
bool InputManager::InputUnit::IsDown() const {
	// �L�[�{�[�h�̊m�F����
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return true;
		}
	}
	// �p�b�h�̊m�F
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return true;
		}
	}
	// �}�E�X�̊m�F
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::HELD) {
			return true;
		}
	}

	return false;
}

/// <summary>
/// ���͂������ォ�擾����
/// </summary>
/// <returns>
/// true : ���͂�������
/// false : ���͂�������łȂ�
/// </returns>
bool InputManager::InputUnit::IsPressed() const {
	bool pressed = false;
	// �L�[�{�[�h�̊m�F����
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(m_keyCode)) {
			pressed = true;
		}
		else if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return false;
		}
	}
	// �p�b�h�̊m�F
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::PRESSED) {
			pressed = true;
		}
		else if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return false;
		}
	}
	// �}�E�X�̊m�F
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			pressed = true;
		}
		else if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::HELD) {
			return false;
		}
	}

	return pressed;
}

/// <summary>
/// ���͂��Ă��Ȃ����擾����
/// </summary>
/// <returns>
/// true : ���͂��Ă��Ȃ�
/// false : ���͂��Ă���
/// </returns>
bool InputManager::InputUnit::IsUp() const {
	// �L�[�{�[�h�̊m�F����
	if (m_keyCode != NONE_USE) {
		if (!ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyUp(m_keyCode)) {
			return false;
		}
	}
	// �p�b�h�̊m�F
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) != DirectX::GamePad::ButtonStateTracker::UP) {
			return false;
		}
	}
	// �}�E�X�̊m�F
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) != DirectX::Mouse::ButtonStateTracker::UP) {
			return false;
		}
	}

	return true;
}

/// <summary>
/// ���͂���߂����ォ�擾����
/// </summary>
/// <returns>
/// true : ���͂���߂�����
/// false : ���͂���߂�����łȂ�
/// </returns>
bool InputManager::InputUnit::IsReleased() const {
	bool released = false;
	// �L�[�{�[�h�̊m�F����
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyReleased(m_keyCode)) {
			released = true;
		}
		else if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return false;
		}
	}
	// �p�b�h�̊m�F
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::RELEASED) {
			released = true;
		}
		else if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return false;
		}
	}
	// �}�E�X�̊m�F
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::RELEASED) {
			released = true;
		}
		else if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::HELD) {
			return false;
		}
	}

	return released;
}


/// <summary>
/// �p�b�h�̐ڑ���Ԃ��擾����
/// </summary>
/// <returns>
/// true : �ڑ����Ă���
/// false : �ڑ����Ă��Ȃ�
/// </returns>
bool InputManager::InputHelper::IsPadConnected() {
	return ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Get()->GetLastState().IsConnected();
}

/// <summary>
/// �p�b�h�̃{�^���̓��͏�Ԃ��擾����
/// </summary>
/// <param name="padCode">�p�b�h�R�[�h</param>
/// <returns>
/// �p�b�h�̃{�^���̓��͏��
/// </returns>
DirectX::GamePad::ButtonStateTracker::ButtonState InputManager::InputHelper::GetPadButtonState(PadCode padCode) {
	DirectX::GamePad::ButtonStateTracker* tracker = ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Get();

	// �{�^�����Ƃɏ����𕪂���
	switch (padCode) {
	case InputManager::PadCode::A:
		return tracker->a;
	case InputManager::PadCode::B:
		return tracker->b;
	case InputManager::PadCode::X:
		return tracker->x;
	case InputManager::PadCode::Y:
		return tracker->y;
	case InputManager::PadCode::LStickLeft:
		return tracker->leftStickLeft;
	case InputManager::PadCode::LStickRight:
		return tracker->leftStickRight;
	case InputManager::PadCode::LStickUp:
		return tracker->leftStickUp;
	case InputManager::PadCode::LStickDown:
		return tracker->leftStickDown;
	case InputManager::PadCode::LShoulder:
		return tracker->leftShoulder;
	case InputManager::PadCode::LTrigger:
		return tracker->leftTrigger;
	case InputManager::PadCode::RShoulder:
		return tracker->rightShoulder;
	case InputManager::PadCode::RTrigger:
		return tracker->rightTrigger;
	case InputManager::PadCode::Start:
		return tracker->start;
	default:
		ErrorMessage(L"�p�b�h�̓��͏�Ԃ̎擾�ŕs���Ȓl���n����܂���");
		break;
	}

	return static_cast<DirectX::GamePad::ButtonStateTracker::ButtonState>(-1);
}

/// <summary>
/// �}�E�X�̃{�^���̓��͏�Ԃ��擾����
/// </summary>
/// <param name="mouseCode">�}�E�X�R�[�h</param>
/// <returns>
/// �}�E�X�̃{�^���̓��͏��
/// </returns>
DirectX::Mouse::ButtonStateTracker::ButtonState InputManager::InputHelper::GetMouseButtonState(MouseCode mouseCode) {
	DirectX::Mouse::ButtonStateTracker* tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();

	// �{�^�����Ƃɏ����𕪂���
	switch (mouseCode) {
	case InputManager::MouseCode::Left:
		return tracker->leftButton;
	case InputManager::MouseCode::Right:
		return tracker->rightButton;
	default:
		ErrorMessage(L"�}�E�X�̓��͏�Ԃ̎擾�ŕs���Ȓl���n����܂���");
		break;
	}

	return static_cast<DirectX::Mouse::ButtonStateTracker::ButtonState>(-1);
}


/// <summary>
/// ���̓R�[�h�̊��蓖��
/// </summary>
/// <param name="id">����ID</param>
/// <param name="keyCode">�Ή�����L�[�R�[�h</param>
/// <param name="padCode">�Ή�����p�b�h�R�[�h</param>
/// <param name="mouseCode">�Ή�����}�E�X�R�[�h</param>
void InputManager::SetInputCode(InputID id, KeyCode keyCode, PadCode padCode, MouseCode mouseCode) {
	m_inputs[static_cast<int>(id)] = InputUnit(keyCode, padCode, mouseCode);
}

/// <summary>
/// �p�b�h�̐ڑ���Ԃ��擾����
/// </summary>
/// <returns>
/// true : �ڑ����Ă���
/// false : �ڑ����Ă��Ȃ�
/// </returns>
bool InputManager::IsPadConnected() const {
	return InputHelper::IsPadConnected();
}

/// <summary>
/// ���͂��Ă��邩�擾����
/// </summary>
/// <param name="id">����ID</param>
/// <returns>
/// true : ���͂��Ă���
/// false : ���͂��Ă��Ȃ�
/// </returns>
bool InputManager::IsDown(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsDown();
}

/// <summary>
/// ���͂������ォ�擾����
/// </summary>
/// <param name="id">����ID</param>
/// <returns>
/// true : ���͂�������
/// false : ���͂�������łȂ�
/// </returns>
bool InputManager::IsPressed(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsPressed();
}

/// <summary>
/// ���͂��Ă��Ȃ����擾����
/// </summary>
/// <param name="id">����ID</param>
/// <returns>
/// true : ���͂��Ă��Ȃ�
/// false : ���͂��Ă���
/// </returns>
bool InputManager::IsUp(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsUp();
}

/// <summary>
/// ���͂���߂����ォ�擾����
/// </summary>
/// <param name="id">����ID</param>
/// <returns>
/// true : ���͂���߂�����
/// false : ���͂���߂�����łȂ�
/// </returns>
bool InputManager::IsReleased(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsReleased();
}
