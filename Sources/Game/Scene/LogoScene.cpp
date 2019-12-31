#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\Fade.h>


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

	// ���\�[�X�����[�h����
	ResourceLoader::Load(ResourceLoaderID::LogoScene);
	// �t�F�[�h�𐶐�����
	m_fade = std::make_unique<Fade>();
	m_fade->Start();
	m_fadeStep = 0;
}

/// <summary>
/// ���S�V�[�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void LogoScene::Update(const DX::StepTimer& timer) {
	// �G�X�P�[�v�L�[�ŏI��
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		ExitGame();
	}

	// �X�y�[�X�L�[�����������N���b�N�A�܂��͎��Ԍo�߂Ń^�C�g���V�[���Ɉڍs����
	const bool press_space = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space);
	const bool click_left = (ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	const bool time_end = (m_fadeStep > 4 && m_fade->IsFinished());
	if (press_space || click_left || time_end) {
		m_pSceneRequest->RequestScene(SceneID::Title);
	}

	// �t�F�[�h���X�V����
	m_fade->Update(timer);
	// �t�F�[�h�̒i�K���X�V����
	if (m_fade->IsFinished() && m_fade->IsRunning()) {
		switch (m_fadeStep) {
		// �����҂�
		case 0:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		// �t�F�[�h�C������
		case 1:
			m_fade->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);
			break;
		// �t�F�[�h����������~�߂�
		case 2:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 0.0f);
			break;
		// �t�F�[�h�A�E�g����
		case 3:
			m_fade->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			break;
		// �t�F�[�h���~�߂�
		case 4:
			m_fade->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		default:
			m_fade->Stop();
			break;
		}
		++m_fadeStep;
	}
	
}

/// <summary>
/// ���S�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Logo);
	
	// ���S��`�悷��
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// ���Ԍo�߂Ńt�F�[�h�C���E�t�F�[�h�A�E�g������
	m_fade->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// ���S�V�[�����I������
/// </summary>
void LogoScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::LogoScene);
}

