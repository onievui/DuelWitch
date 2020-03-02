#include "InputManager.h"
#include <Utils\ErrorMessage.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MouseWrapper.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="keyCode">対応するキーコード</param>
/// <param name="padCode">対応するパッドコード</param>
/// <param name="mouseCode">対応するマウスコード</param>
InputManager::InputUnit::InputUnit(KeyCode keyCode, PadCode padCode, MouseCode mouseCode) 
	: m_keyCode(keyCode)
	, m_padCode(padCode)
	, m_mouseCode(mouseCode) {
}

/// <summary>
/// 入力しているか取得する
/// </summary>
/// <returns>
/// true : 入力している
/// false : 入力していない
/// </returns>
bool InputManager::InputUnit::IsDown() const {
	// キーボードの確認する
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return true;
		}
	}
	// パッドの確認
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return true;
		}
	}
	// マウスの確認
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) == DirectX::Mouse::ButtonStateTracker::HELD) {
			return true;
		}
	}

	return false;
}

/// <summary>
/// 入力した直後か取得する
/// </summary>
/// <returns>
/// true : 入力した直後
/// false : 入力した直後でない
/// </returns>
bool InputManager::InputUnit::IsPressed() const {
	bool pressed = false;
	// キーボードの確認する
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(m_keyCode)) {
			pressed = true;
		}
		else if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return false;
		}
	}
	// パッドの確認
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::PRESSED) {
			pressed = true;
		}
		else if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return false;
		}
	}
	// マウスの確認
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
/// 入力していないか取得する
/// </summary>
/// <returns>
/// true : 入力していない
/// false : 入力している
/// </returns>
bool InputManager::InputUnit::IsUp() const {
	// キーボードの確認する
	if (m_keyCode != NONE_USE) {
		if (!ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyUp(m_keyCode)) {
			return false;
		}
	}
	// パッドの確認
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) != DirectX::GamePad::ButtonStateTracker::UP) {
			return false;
		}
	}
	// マウスの確認
	if (m_mouseCode != static_cast<MouseCode>(NONE_USE)) {
		if (InputHelper::GetMouseButtonState(m_mouseCode) != DirectX::Mouse::ButtonStateTracker::UP) {
			return false;
		}
	}

	return true;
}

/// <summary>
/// 入力をやめた直後か取得する
/// </summary>
/// <returns>
/// true : 入力をやめた直後
/// false : 入力をやめた直後でない
/// </returns>
bool InputManager::InputUnit::IsReleased() const {
	bool released = false;
	// キーボードの確認する
	if (m_keyCode != NONE_USE) {
		if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyReleased(m_keyCode)) {
			released = true;
		}
		else if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->GetLastState().IsKeyDown(m_keyCode)) {
			return false;
		}
	}
	// パッドの確認
	if (m_padCode != static_cast<PadCode>(NONE_USE) && InputHelper::IsPadConnected()) {
		if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::RELEASED) {
			released = true;
		}
		else if (InputHelper::GetPadButtonState(m_padCode) == DirectX::GamePad::ButtonStateTracker::HELD) {
			return false;
		}
	}
	// マウスの確認
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
/// パッドの接続状態を取得する
/// </summary>
/// <returns>
/// true : 接続している
/// false : 接続していない
/// </returns>
bool InputManager::InputHelper::IsPadConnected() {
	return ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Get()->GetLastState().IsConnected();
}

/// <summary>
/// パッドのボタンの入力状態を取得する
/// </summary>
/// <param name="padCode">パッドコード</param>
/// <returns>
/// パッドのボタンの入力状態
/// </returns>
DirectX::GamePad::ButtonStateTracker::ButtonState InputManager::InputHelper::GetPadButtonState(PadCode padCode) {
	DirectX::GamePad::ButtonStateTracker* tracker = ServiceLocater<DirectX::GamePad::ButtonStateTracker>::Get();

	// ボタンごとに処理を分ける
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
		ErrorMessage(L"パッドの入力状態の取得で不正な値が渡されました");
		break;
	}

	return static_cast<DirectX::GamePad::ButtonStateTracker::ButtonState>(-1);
}

/// <summary>
/// マウスのボタンの入力状態を取得する
/// </summary>
/// <param name="mouseCode">マウスコード</param>
/// <returns>
/// マウスのボタンの入力状態
/// </returns>
DirectX::Mouse::ButtonStateTracker::ButtonState InputManager::InputHelper::GetMouseButtonState(MouseCode mouseCode) {
	DirectX::Mouse::ButtonStateTracker* tracker = ServiceLocater<MouseWrapper>::Get()->GetTracker();

	// ボタンごとに処理を分ける
	switch (mouseCode) {
	case InputManager::MouseCode::Left:
		return tracker->leftButton;
	case InputManager::MouseCode::Right:
		return tracker->rightButton;
	default:
		ErrorMessage(L"マウスの入力状態の取得で不正な値が渡されました");
		break;
	}

	return static_cast<DirectX::Mouse::ButtonStateTracker::ButtonState>(-1);
}


/// <summary>
/// 入力コードの割り当て
/// </summary>
/// <param name="id">入力ID</param>
/// <param name="keyCode">対応するキーコード</param>
/// <param name="padCode">対応するパッドコード</param>
/// <param name="mouseCode">対応するマウスコード</param>
void InputManager::SetInputCode(InputID id, KeyCode keyCode, PadCode padCode, MouseCode mouseCode) {
	m_inputs[static_cast<int>(id)] = InputUnit(keyCode, padCode, mouseCode);
}

/// <summary>
/// パッドの接続状態を取得する
/// </summary>
/// <returns>
/// true : 接続している
/// false : 接続していない
/// </returns>
bool InputManager::IsPadConnected() const {
	return InputHelper::IsPadConnected();
}

/// <summary>
/// 入力しているか取得する
/// </summary>
/// <param name="id">入力ID</param>
/// <returns>
/// true : 入力している
/// false : 入力していない
/// </returns>
bool InputManager::IsDown(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsDown();
}

/// <summary>
/// 入力した直後か取得する
/// </summary>
/// <param name="id">入力ID</param>
/// <returns>
/// true : 入力した直後
/// false : 入力した直後でない
/// </returns>
bool InputManager::IsPressed(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsPressed();
}

/// <summary>
/// 入力していないか取得する
/// </summary>
/// <param name="id">入力ID</param>
/// <returns>
/// true : 入力していない
/// false : 入力している
/// </returns>
bool InputManager::IsUp(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsUp();
}

/// <summary>
/// 入力をやめた直後か取得する
/// </summary>
/// <param name="id">入力ID</param>
/// <returns>
/// true : 入力をやめた直後
/// false : 入力をやめた直後でない
/// </returns>
bool InputManager::IsReleased(InputID id) const {
	return m_inputs[static_cast<int>(id)].IsReleased();
}
