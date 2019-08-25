#include "CharaSelectScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
CharaSelectScene::CharaSelectScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CharaSelectScene::~CharaSelectScene() {
}

/// <summary>
///	�L�����Z���N�g�V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void CharaSelectScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

}

/// <summary>
/// �L�����Z���N�g�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void CharaSelectScene::Update(const DX::StepTimer& timer) {
	timer;

	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		m_pSceneRequest->RequestScene("Play");
	}
}

/// <summary>
/// �L�����Z���N�g�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void CharaSelectScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"CharaSelectScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// �L�����Z���N�g�V�[�����I������
/// </summary>
void CharaSelectScene::Finalize() {

}

