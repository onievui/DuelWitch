#pragma once
#ifndef INPUT_MANAGER_DEFINED
#define INPUT_MANAGER_DEFINED


#include "InputID.h"


/// <summary>
/// ���̓}�l�[�W���N���X
/// </summary>
class InputManager {
public:
	// �L�[�R�[�h
	using KeyCode = DirectX::Keyboard::Keys;
	// �p�b�h�R�[�h
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
	// �}�E�X�R�[�h
	enum class MouseCode {
		Left,
		Right,

		Num
	};
	// ���g�p�̒�`
	static constexpr int NONE_USE = -1;

private:
	/// <summary>
	/// ���͒P�ʃN���X
	/// </summary>
	class InputUnit {
	public:
		// �R���X�g���N�^
		InputUnit(KeyCode keyCode = static_cast<KeyCode>(NONE_USE),
			PadCode padCode = static_cast<PadCode>(NONE_USE),
			MouseCode mouseCode = static_cast<MouseCode>(NONE_USE));

	public:
		// ���͂��Ă��邩�擾����
		bool IsDown() const;
		// ���͂������ォ��������
		bool IsPressed() const;
		// ���͂��Ă��Ȃ����擾����
		bool IsUp() const;
		// ���͂���߂����ォ�擾����
		bool IsReleased() const;

	private:
		// �Ή�����L�[�{�[�h
		KeyCode   m_keyCode;
		// �Ή�����p�b�h
		PadCode   m_padCode;
		// �Ή�����}�E�X
		MouseCode m_mouseCode;
	};

	/// <summary>
	/// ���̓w���p�N���X
	/// </summary>
	class InputHelper {
	public:
		// �p�b�h�̐ڑ���Ԃ��擾����
		static bool IsPadConnected();
		// �p�b�h�̃{�^���̓��͏�Ԃ��擾����
		static DirectX::GamePad::ButtonStateTracker::ButtonState GetPadButtonState(PadCode padCode);
		// �}�E�X�̃{�^���̓��͏�Ԃ��擾����
		static DirectX::Mouse::ButtonStateTracker::ButtonState GetMouseButtonState(MouseCode mouseCode);
	};

public:
	// ���̓R�[�h�̊��蓖��
	void SetInputCode(InputID id,
		KeyCode keyCode = static_cast<KeyCode>(NONE_USE),
		PadCode padCode = static_cast<PadCode>(NONE_USE),
		MouseCode mouseCode = static_cast<MouseCode>(NONE_USE));
	// ���͂��Ă��邩�擾����
	bool IsDown(InputID id) const;
	// ���͂������ォ��������
	bool IsPressed(InputID id) const;
	// ���͂��Ă��Ȃ����擾����
	bool IsUp(InputID id) const;
	// ���͂���߂����ォ�擾����
	bool IsReleased(InputID id) const;

private:
	// �e���͒P��
	std::array<InputUnit, static_cast<int>(InputID::Num)> m_inputs;
};


#endif // !INPUT_MANAGER_DEFINED
