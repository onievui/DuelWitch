#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
LogoScene::LogoScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LogoScene::~LogoScene() {
}

/// <summary>
///	���S�V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void LogoScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());
	// �X�v���C�g�t�H���g�𐶐�����
	//m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");

	// �L�[�g���b�J�[�𐶐�����
	m_tracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

}

/// <summary>
/// ���S�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void LogoScene::Update(const DX::StepTimer& timer) {
	timer;
	m_tracker->Update(DirectX::Keyboard::Get().GetState());

	if (m_tracker->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		m_pSceneRequest->RequestScene("Play");
	}
}

/// <summary>
/// ���S�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	spriteBatch->End();
}

/// <summary>
/// ���S�V�[�����I������
/// </summary>
void LogoScene::Finalize() {

}

