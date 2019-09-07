#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


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
/// <param name="pSceneRequest">���N�G�X�g�V�[���C���^�t�F�[�X</param>
void LogoScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	ResourceLoader::Load(ResourceLoaderID::LogoScene);
	m_time = 0.0f;

}

/// <summary>
/// ���S�V�[�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void LogoScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// �X�y�[�X�L�[��������5�b�o�߂Ń^�C�g���V�[���Ɉڍs����
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space) ||
		m_time > 5.0f) {
		m_pSceneRequest->RequestScene("Title");
	}
}

/// <summary>
/// ���S�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Logo);
	// ���Ԍo�߂Ńt�F�[�h�C���A�A�E�g���s��
	float alpha;
	if (m_time <= 2.0f) {
		alpha = std::max(m_time - 1.0f, 0.0f);
	}
	else if (m_time <= 3.0f) {
		alpha = 1.0f;
	}
	else {
		alpha = std::max(4.0f - m_time, 0.0f);
	}
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		DirectX::SimpleMath::Vector2(320.0f, 240.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"LogoScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// ���S�V�[�����I������
/// </summary>
void LogoScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::LogoScene);
}

