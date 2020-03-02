#pragma once
#ifndef INPUT_MANAGER_DEFINED
#define INPUT_MANAGER_DEFINED


#include "InputID.h"


/// <summary>
/// 入力マネージャクラス
/// </summary>
class InputManager {
public:
	// キーコード
	using KeyCode = DirectX::Keyboard::Keys;
	// パッドコード
	enum class PadCode {
		A,
		B,
		X,
		Y,
		LStickLeft,
		LStickRight,
		LStickUp,
		LStickDown,
		LShoulder,
		LTrigger,
		RShoulder,
		RTrigger,
		Start,

		Num
	};
	// マウスコード
	enum class MouseCode {
		Left,
		Right,

		Num
	};
	// 未使用の定義
	static constexpr int NONE_USE = -1;

private:
	/// <summary>
	/// 入力単位クラス
	/// </summary>
	class InputUnit {
	public:
		// コンストラクタ
		InputUnit(KeyCode keyCode = static_cast<KeyCode>(NONE_USE),
			PadCode padCode = static_cast<PadCode>(NONE_USE),
			MouseCode mouseCode = static_cast<MouseCode>(NONE_USE));

	public:
		// 入力しているか取得する
		bool IsDown() const;
		// 入力した直後か所得する
		bool IsPressed() const;
		// 入力していないか取得する
		bool IsUp() const;
		// 入力をやめた直後か取得する
		bool IsReleased() const;

	private:
		// 対応するキーボード
		KeyCode   m_keyCode;
		// 対応するパッド
		PadCode   m_padCode;
		// 対応するマウス
		MouseCode m_mouseCode;
	};

	/// <summary>
	/// 入力ヘルパクラス
	/// </summary>
	class InputHelper {
	public:
		// パッドの接続状態を取得する
		static bool IsPadConnected();
		// パッドのボタンの入力状態を取得する
		static DirectX::GamePad::ButtonStateTracker::ButtonState GetPadButtonState(PadCode padCode);
		// マウスのボタンの入力状態を取得する
		static DirectX::Mouse::ButtonStateTracker::ButtonState GetMouseButtonState(MouseCode mouseCode);
	};

public:
	// 入力コードの割り当て
	void SetInputCode(InputID id,
		KeyCode keyCode = static_cast<KeyCode>(NONE_USE),
		PadCode padCode = static_cast<PadCode>(NONE_USE),
		MouseCode mouseCode = static_cast<MouseCode>(NONE_USE));
	// 入力しているか取得する
	bool IsDown(InputID id) const;
	// 入力した直後か所得する
	bool IsPressed(InputID id) const;
	// 入力していないか取得する
	bool IsUp(InputID id) const;
	// 入力をやめた直後か取得する
	bool IsReleased(InputID id) const;

private:
	// 各入力単位
	std::array<InputUnit, static_cast<int>(InputID::Num)> m_inputs;
};


#endif // !INPUT_MANAGER_DEFINED
