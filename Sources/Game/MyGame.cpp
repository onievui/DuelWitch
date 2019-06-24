#define _CRT_SECURE_NO_WARNINGS

#include "MyGame.h"
#include "SceneManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
MyGame::MyGame(int width, int height) : m_width(width), m_height(height), Game(width, height) {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MyGame::~MyGame() {
}

/// <summary>
/// �Q�[��������������
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
void MyGame::Initialize(int width, int height) {
	// ���N���X��Initialize���Ăяo�� 
	Game::Initialize(width, height);

	// �L�[�{�[�h�𐶐�����
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	// �}�E�X�𐶐�����
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(DirectX11::Get().GetHWnd());
	// �V�[���}�l�[�W��������������
	m_sceneManager = std::make_unique<SceneManager>();
	m_sceneManager->Initialize();
	// �v���C�V�[�����Ăяo��
	m_sceneManager->RequestScene("Play");

}

/// <summary>
/// ���\�[�X�𐶐�����
/// </summary>
void MyGame::CreateResources() {
	// ���N���X��CreateResources���Ăяo��
	Game::CreateResources();

}

// �Q�[�����X�V����
void MyGame::Update(const DX::StepTimer& timer) 
{
	// �V�[�����X�V����
	m_sceneManager->Update(timer);
	
}

// �Q�[����`�悷��
void MyGame::Render(const DX::StepTimer& timer) 
{
	// �ŏ��̍X�V�̑O�͉����`�悵�Ȃ��悤�ɂ���
	if (timer.GetFrameCount() == 0) 
		return;

	// TODO: �����_�����O�R�[�h��ǉ�����
	float time = float(timer.GetTotalSeconds());

	// �o�b�t�@���N���A����
	Clear();

	// �V�[����`�悷��
	m_sceneManager->Render(GetSpriteBatch());

	// �o�b�N�o�b�t�@��\������
	Present();
}

// ��n��������
void MyGame::Finalize() 
{
	// ���N���X��Finalize���Ăяo��
	Game::Finalize();

	m_keyboard.reset();
	m_mouse.reset();
}

// FPS��`�悷��
void MyGame::DrawFPS(const DX::StepTimer& timer)
{
	// FPS������𐶐�����
	std::wstring fpsString = L"fps = " + std::to_wstring((unsigned int)timer.GetFramesPerSecond());
	// FPS��`�悷��
	GetSpriteFont()->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
