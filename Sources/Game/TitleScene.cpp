#include "TitleScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleScene::TitleScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleScene::~TitleScene() {
}

/// <summary>
///	�^�C�g���V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void TitleScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	ResourceLoader::Load(ResourceLoaderID::TitleScene);
	m_time = 0.0f;
}

/// <summary>
/// �^�C�g���V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// �X�y�[�X�L�[���������ꍇ�̏���
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		// �^�C�g�����\������Ă��Ȃ���΁A�^�C�}�[��i�߂ĕ\��������
		if (m_time < 2.0f) {
			m_time = 2.0f;
		}
		// �L�����Z���N�g�V�[���ɐi��
		else {
			m_pSceneRequest->RequestScene("CharaSelect");
		}
	}
}

/// <summary>
/// �^�C�g���V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void TitleScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Title);
	// ���Ԍo�߂Ńt�F�[�h�C�����s��
	float alpha;
	if (m_time <= 2.0f) {
		alpha = std::max(m_time - 1.0f, 0.0f);
	}
	else {
		alpha = 1.0f;
	}
	// �^�C�g����`�悷��
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f)*1.25f);

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"TitleScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// �^�C�g���V�[�����I������
/// </summary>
void TitleScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::TitleScene);
}

