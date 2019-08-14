#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED


#include <Framework\Window.h>
#include <Framework\Game.h>


class SceneManager;


class MyGame : public Game {
public:
	// �R���X�g���N�^
	MyGame(int width, int height);
	// �f�X�g���N�^
	~MyGame();
	// �Q�[��������������
	void Initialize(int width, int height) override;
	// ���\�[�X�𐶐�����
	void CreateResources() override;
	// �Q�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �Q�[����`�悷��
	void Render(const DX::StepTimer& timer) override;
	// �I�������������Ȃ�
	void Finalize() override;

	// FPS��`�悷��
	void DrawFPS(const DX::StepTimer& timer);

private:
	// ��
	int m_width;
	// ����
	int m_height;

	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// �V�[���}�l�[�W��
	std::unique_ptr<SceneManager> m_sceneManager;

};

#endif	// MYGAME_DEFINED