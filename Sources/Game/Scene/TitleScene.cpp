#include "TitleScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\AudioManager.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\SoundScaleUpUI.h>
#include <Game\UI\Fade.h>


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

	// ���\�[�X�����[�h����
	ResourceLoader::Load(ResourceLoaderID::TitleScene);
	m_wasSelected = false;

	// UI������������
	InitializeUI();

	// �t�F�[�h�𐶐�����
	m_fadeScreen = std::make_unique<Fade>();
	m_fadeScreen->Start();
	m_fadeScreenStep = 0;

	m_fadeUI = std::make_unique<Fade>();
	// �ォ�瓮����J�n������
	m_fadeUI->Stop();
	m_fadeUIStep = 0;

	// BGM���Đ�����
	ServiceLocater<AudioManager>::Get()->PlayBgm(BgmID::Title);
	// BGM���t�F�[�h�C��������
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Title, 0, 0.9f, 0.0f, 1.0f);
}

/// <summary>
/// �^�C�g���V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	// �G�X�P�[�v�L�[�ŏI��
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		ExitGame();
	}

	// �X�y�[�X�L�[�����������N���b�N�����ꍇ
	const bool press_space = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space);
	const bool click_left = (ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED);
	if (press_space || click_left) {
		// �^�C�g�����\������Ă��Ȃ���΁A�^�C�}�[��i�߂ĕ\��������
		if (m_fadeScreenStep < 3) {
			m_fadeScreenStep = 2;
			m_fadeScreen->SkipTime();
		}
	}

	// UI�̃A���t�@�l���X�V����
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// ���I���Ńt�F�[�h���������Ă�����UI���X�V����
	const bool ui_fade_end = (m_fadeUIStep == 2);
	if (!m_wasSelected && ui_fade_end) {
		for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// �C�x���g���擾���Ă��邩�ǂ����m�F����
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// �C�x���g�ɉ����ăV�[����؂�ւ���
			switch (event_id) {
			case UIEventID::Tutorial:
				ErrorMessage(L"������");
				break;
			case UIEventID::Play:
				// �L�����Z���N�g�V�[���ɐi��
				m_nextSceneID = SceneID::CharaSelect;
				// �t�F�[�h�A�E�g��ɃV�[���J�ڂ���
				m_fadeScreen->Start();
				m_wasSelected = true;
				// BGM���t�F�[�h�A�E�g������
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Title, 0, 0.9f, 1.0f, 0.0f);
				break;
			case UIEventID::Option:
				ErrorMessage(L"������");
				break;
				// �Q�[�����I������
			case UIEventID::Exit:
				ExitGame();
				break;
			default:
				ErrorMessage(L"�s����UI�C�x���g���擾���܂���");
				break;
			}
		}
	}

	// ��ʗp�t�F�[�h���X�V����
	m_fadeScreen->Update(timer);
	// ��ʗp�t�F�[�h�̒i�K���X�V����
	if (m_fadeScreen->IsFinished() && m_fadeScreen->IsRunning()) {
		switch (m_fadeScreenStep) {
		// �����҂�
		case 0:
			m_fadeScreen->Initialize(Fade::State::NoFade, 1.0f, 1.0f);
			break;
		// �t�F�[�h�C������
		case 1:
			m_fadeScreen->Initialize(Fade::State::FadeIn, 1.0f, 0.0f);
			break;
		// �t�F�[�h�A�E�g�̏���������
		case 2:
			m_fadeScreen->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
			m_fadeScreen->Stop();
			// UI�̃t�F�[�h���J�n����
			m_fadeUI->Start();
			break;
		// �t�F�[�h�A�E�g������������V�[���J�ڂ���
		case 3:
			m_pSceneRequest->RequestScene(m_nextSceneID);
			break;
		default:
			m_fadeScreen->Stop();
			break;
		}
		++m_fadeScreenStep;
	}

	// UI�p�t�F�[�h���X�V����
	m_fadeUI->Update(timer);
	// UI�p�t�F�[�h�̒i�K���X�V����
	if (m_fadeUI->IsFinished() && m_fadeUI->IsRunning()) {
		switch (m_fadeUIStep) {
		// UI���t�F�[�h�C��������
		case 0:
			m_fadeUI->Initialize(Fade::State::FadeInAlpha, 1.5f, 1.0f);
			break;
		case 1:
			m_fadeUI->Stop();
		default:
			break;
		}
		++m_fadeUIStep;
	}
}

/// <summary>
/// �^�C�g���V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void TitleScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Title);
	
	// �^�C�g����`�悷��
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::Colors::White, 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UI��`�悷��
	if (m_fadeUI->GetAlpha() > 0.0f) {
		for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}

	// ���Ԍo�߂Ńt�F�[�h�C���E�t�F�[�h�A�E�g������
	m_fadeScreen->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// �^�C�g���V�[�����I������
/// </summary>
void TitleScene::Finalize() {
	// UI����I�u�U�[�o���f�^�b�`����
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Detach(m_uiObserver.get());
	}
	ResourceLoader::Release(ResourceLoaderID::TitleScene);
}

/// <summary>
/// UI������������
/// </summary>
void TitleScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));
	
	// UI�̐���
	// �`���[�g���A��
	//{
	//	std::unique_ptr<SoundScaleUpUI> tutorial = std::make_unique<SoundScaleUpUI>(
	//		UIEventID::Tutorial, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.4f));
	//	tutorial->SetText(L"Tutorial");
	//	m_menuUIs.emplace_back(std::move(tutorial));
	//}
	// �v���C
	{
		std::unique_ptr<SoundScaleUpUI> play = std::make_unique<SoundScaleUpUI>(
			UIEventID::Play, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.55f));
		play->SetText(L"Play");
		m_menuUIs.emplace_back(std::move(play));
	}
	// �I�v�V����
	//{
	//	std::unique_ptr<SoundScaleUpUI> option = std::make_unique<SoundScaleUpUI>(
	//		UIEventID::Option, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.7f));
	//	option->SetText(L"Option");
	//	m_menuUIs.emplace_back(std::move(option));
	//}
	// �I��
	{
		std::unique_ptr<SoundScaleUpUI> exit = std::make_unique<SoundScaleUpUI>(
			UIEventID::Exit, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.85f));
		exit->SetText(L"Exit");
		m_menuUIs.emplace_back(std::move(exit));
	}

	// ���ʂ̏���
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<SoundScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UI�ɃI�u�U�[�o���A�^�b�`����
		(*itr)->Attach(m_uiObserver.get());
	}
}

