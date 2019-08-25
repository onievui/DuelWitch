#define _CRT_SECURE_NO_WARNINGS


#include "MyGame.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include "CommonServices.h"
#include "SceneManager.h"
#include "ResourceLoader.h"


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
	m_mouse->SetWindow(ServiceLocater<DirectX11>::Get()->GetHWnd());

	// �R�����T�[�r�X������������
	m_commonServices = std::make_unique<CommonServices>();
	m_commonServices->Initialize();

	// �t�H���g��ǂݍ���
	ResourceLoader::Load(ResourceLoaderID::Common);

	// �V�[���}�l�[�W��������������
	m_sceneManager = std::make_unique<SceneManager>();
	m_sceneManager->Initialize();

	// �v���C�V�[�����Ăяo��
	m_sceneManager->RequestScene("Logo");
}

/// <summary>
/// ���\�[�X�𐶐�����
/// </summary>
void MyGame::CreateResources() {
	// ���N���X��CreateResources���Ăяo��
	Game::CreateResources();

}

// �Q�[�����X�V����
void MyGame::Update(const DX::StepTimer& timer) {
	// �L�[�g���b�J�[���X�V����
	ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Update(m_keyboard->GetState());
	// �}�E�X�g���b�J�[���X�V����
	ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get()->Update(m_mouse->GetState());

	// �V�[�����X�V����
	m_sceneManager->Update(timer);
	
}

// �Q�[����`�悷��
void MyGame::Render(const DX::StepTimer& timer) {
	// �ŏ��̍X�V�̑O�͉����`�悵�Ȃ��悤�ɂ���
	if (timer.GetFrameCount() == 0) {
		return;
	}

	// �o�b�t�@���N���A����
	Clear();

	// �V�[����`�悷��
	m_sceneManager->Render(GetSpriteBatch());

	// �o�b�N�o�b�t�@��\������
	Present();
}

// ��n��������
void MyGame::Finalize() {
	// ���N���X��Finalize���Ăяo��
	Game::Finalize();

	ResourceLoader::Release(ResourceLoaderID::Common);
	m_commonServices->Finalize();

	m_keyboard.reset();
	m_mouse.reset();
}

// FPS��`�悷��
void MyGame::DrawFPS(const DX::StepTimer& timer) {
	// FPS������𐶐�����
	std::wstring fpsString = L"fps = " + std::to_wstring(static_cast<unsigned int>(timer.GetFramesPerSecond()));
	// FPS��`�悷��
	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
