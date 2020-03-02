#include "PauseScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\LoadDataManager.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\MenuUI.h>
#include <Game\UI\Fade.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
PauseScene::PauseScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PauseScene::~PauseScene() {
}

/// <summary>
///	�|�[�Y�V�[��������������
/// </summary>
/// <param name="pSceneRequest">���N�G�X�g�V�[���C���^�t�F�[�X</param>
void PauseScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// UI������������
	InitializeUI();

	// �}�E�X�J�[�\����\������
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
	ServiceLocater<MouseWrapper>::Get()->SetVisible(true);
	// �}�E�X���E�C���h�E���Ɉړ��ł���悤�ɂ���
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(false);

	// �t�F�[�h�𐶐�����
	m_fadeBack = std::make_unique<Fade>();
	m_fadeBack->Initialize(Fade::State::FadeOut, PAUSE_TIME, 0.8f);

	m_fadeFront = std::make_unique<Fade>();
	m_fadeFront->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
	// �ォ�瓮����J�n������
	m_fadeFront->Stop();

	m_fadeUI = std::make_unique<Fade>();
	m_fadeUI->Initialize(Fade::State::FadeInAlpha, PAUSE_TIME, 1.0f);

	m_wasSelected = false;

	// �|�[�Y����炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Pause);

	// BGM�̉��ʂ�������
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, PAUSE_TIME, 1.0f, BGM_VOLUME);

}

/// <summary>
/// �|�[�Y�V�[�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PauseScene::Update(const DX::StepTimer& timer) {
	
	m_fadeBack->Update(timer);
	m_fadeFront->Update(timer);
	m_fadeUI->Update(timer);

	// UI�̃A���t�@�l���X�V����
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// �t�F�[�h����������܂ŏ������Ȃ�
	if (!m_fadeBack->IsFinished()) {
		return;
	}

	// F2�L�[�Ńp�����[�^���ēǂݍ��݂���
	const bool press_f2 = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::F2);
	if (press_f2) {
		LoadDataManager::GetIns()->Reload(LoadDataID::PlayScene);
	}

	// �G�X�P�[�v�L�[�������āA�v���C�V�[�����ĊJ����
	const bool press_escape = ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape);
	if (press_escape) {
		SelectResume();
	}

	// ���I���Ńt�F�[�h���������Ă�����UI���X�V����
	if (!m_wasSelected && m_fadeUI->IsFinished()) {
		// UI���X�V����
		for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// �C�x���g���擾���Ă��邩�ǂ����m�F����
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// �C�x���g�ɉ����ăV�[����؂�ւ���
			switch (event_id) {
				// �ĊJ����
			case UIEventID::Resume:
				SelectResume();
				return;
				// �L�����Z���N�g�ɖ߂�
			case UIEventID::CharaSelect:
				m_nextSceneID = SceneID::CharaSelect;
				m_fadeFront->Start();
				// BGM���t�F�[�h�A�E�g������
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, BGM_VOLUME, 0.0f);
				break;
				// �^�C�g���ɖ߂�
			case UIEventID::Title:
				m_nextSceneID = SceneID::Title;
				m_fadeFront->Start();
				// BGM���t�F�[�h�A�E�g������
				ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, 0.9f, BGM_VOLUME, 0.0f);
				break;
			default:
				ErrorMessage(L"�s����UI�C�x���g���擾���܂���");
				break;
			}
			m_wasSelected = true;
		}
	}

	// �I�����Ă����莞�Ԍo�߂�����V�[����J�ڂ���
	if (m_fadeFront->IsFinished()) {
		//�v���C�V�[����I�����Ă�����|�[�Y����������
		if (m_nextSceneID == SceneID::Play) {
			Resume();
		}
		// ����ȊO�Ȃ�A���̃V�[���ɑJ�ڂ���
		else {
			m_pSceneRequest->RequestScene(m_nextSceneID);
		}
	}

}

/// <summary>
/// �|�[�Y�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void PauseScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// �v���C��ʂɍ����d�˂ĈÂ�����
	m_fadeBack->Render(spriteBatch);

	// UI��`�悷��
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	
	// �ĊJ���Ȃ��ꍇ�̓t�F�[�h�A�E�g����
	m_fadeFront->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// �|�[�Y�V�[�����I������
/// </summary>
void PauseScene::Finalize() {
	// UI����I�u�U�[�o���f�^�b�`����
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Detach(m_uiObserver.get());
	}
}

/// <summary>
/// UI������������
/// </summary>
void PauseScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UI�̐���
	// �ĊJ
	{
		std::unique_ptr<MenuUI> resume = std::make_unique<MenuUI>(
			UIEventID::Resume, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.25f));
		resume->SetText(L"Resume");
		m_menuUIs.emplace_back(std::move(resume));
	}
	// �L�����Z���N�g
	{
		std::unique_ptr<MenuUI> charaselect = std::make_unique<MenuUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.5f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// �^�C�g�� 
	{
		std::unique_ptr<MenuUI> title = std::make_unique<MenuUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.75f));
		title->SetText(L"Title");
		m_menuUIs.emplace_back(std::move(title));
	}
	
	// ���ʂ̏���
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UI�ɃI�u�U�[�o���A�^�b�`����
		(*itr)->Attach(m_uiObserver.get());
	}
}

/// <summary>
/// �|�[�Y�̉�����I������
/// </summary>
void PauseScene::SelectResume() {
	// �A���t�@�l��0.8����n�܂�悤�ɒ�������
	m_fadeBack->Initialize(Fade::State::FadeIn, PAUSE_TIME*1.25f, 0.0f);
	m_fadeBack->SkipTime(0.2f);

	m_fadeUI->Initialize(Fade::State::FadeOutAlpha, PAUSE_TIME, 0.0f);

	// �I������̂��߂Ɏ�O��ʗp�t�F�[�h���^�C�}�[�Ƃ��Ĉ���
	m_fadeFront->Initialize(Fade::State::NoFade, PAUSE_TIME, 0.0f);

	m_wasSelected = true;
	m_nextSceneID = SceneID::Play;

	// BGM�̉��ʂ�߂�
	ServiceLocater<AudioManager>::Get()->FadeBgm(BgmID::Battle, 0, PAUSE_TIME, BGM_VOLUME, 1.0f);
}

/// <summary>
/// �|�[�Y����������
/// </summary>
void PauseScene::Resume() {
	m_pSceneRequest->PopScene();
	// �}�E�X�J�[�\�����\���ɖ߂�
	//ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
	ServiceLocater<MouseWrapper>::Get()->SetVisible(false);
	// �}�E�X���E�C���h�E���Ɏ��߂�
	ServiceLocater<MouseWrapper>::Get()->ClipToWindow(true);
}

