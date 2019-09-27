#pragma once
#ifndef MOUSE_WRAPPER_DEFINED
#define MOUSE_WRAPPER_DEFINED


/// <summary>
/// �}�E�X���b�p�[�N���X
/// </summary>
class MouseWrapper {
public:
	// �R���X�g���N�^
	MouseWrapper(HWND hWnd);
	// �f�X�g���N�^
	~MouseWrapper();

public:
	// �}�E�X���X�V����
	void Update(int width, int height);
	// �}�E�X�̏�Ԃ����Z�b�g����
	void Reset();

public:
	// �}�E�X���擾����
	DirectX::Mouse* GetMouse();
	// �g���b�J�[���擾����
	DirectX::Mouse::ButtonStateTracker* GetTracker();
	// �}�E�X���x��ݒ肷��
	void SetSensivity(float sensivity);
	// �}�E�X���x���擾����
	float GetSensivity() const;
	// �}�E�X�̍��W���擾����
	const DirectX::SimpleMath::Vector2& GetPos() const;
	// �}�E�X�̍��W��ݒ肷��
	void SetPos(const DirectX::SimpleMath::Vector2& pos);
	// �}�E�X�̈ړ��ʂ��擾����
	const DirectX::SimpleMath::Vector2& GetMoved() const;
	// �}�E�X�̍��W���[�h��ݒ肷��
	void SetMode(DirectX::Mouse::Mode mode);

private:
	// �}�E�X�{��
	std::unique_ptr<DirectX::Mouse>                     m_mouse;
	// �}�E�X�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_tracker;
	// �}�E�X���x
	float                                               m_sensivity;
	// �}�E�X���W
	DirectX::SimpleMath::Vector2                        m_pos;
	// �}�E�X�ړ���
	DirectX::SimpleMath::Vector2                        m_moved;
	// �}�E�X�̍��W���[�h
	DirectX::Mouse::Mode                                m_mode;
	// �}�E�X�𑊑΃��[�h�����΃��[�h�ɕύX�������̃t���O
	bool                                                m_wasChangedToAbsolute;
	// ��΃��[�h�ł̃}�E�X�̍ŏI�ʒu
	DirectX::SimpleMath::Vector2                        m_preAbsolutePos;
};


#endif // !MOUSE_WRAPPER_DEFINED
