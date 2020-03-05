#include "WinResultScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\MouseWrapper.h>
#include "ISceneRequest.h"
#include <Game\UI\MenuUI.h>
#include <Game\UI\Fade.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
WinResultScene::WinResultScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
WinResultScene::~WinResultScene() {
}

/// <summary>
///	�������U���g�V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void WinResultScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	// UI������������
	InitializeUI();

	// �}�E�X�J�[�\����\������
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);

	// �t�F�[�h�𐶐�����
	m_fadeScreen = std::make_unique<Fade>();
	m_fadeScreen->Initialize(Fade::State::FadeOut, 1.0f, 1.0f);
	// �ォ�瓮����J�n������
	m_fadeScreen->Stop();

	m_fadeLogo = std::make_unique<Fade>();
	m_fadeLogo->Initialize(Fade::State::FadeInAlpha, 1.0f, 1.0f);

	m_fadeUI = std::make_unique<Fade>();
	m_fadeUI->Initialize(Fade::State::FadeInAlpha, 1.0f, 1.0f);
	// �ォ�瓮����J�n������
	m_fadeUI->Stop();
	m_fadeUIFinished = false;

	m_wasSelected = false;
}

/// <summary>
/// �������U���g�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void WinResultScene::Update(const DX::StepTimer& timer) {

	// �t�F�[�h���X�V����
	m_fadeScreen->Update(timer);
	m_fadeLogo->Update(timer);
	m_fadeUI->Update(timer);

	// ���U���g���S�̃t�F�[�h������������UI�̃t�F�[�h���J�n������
	if (m_fadeLogo->IsFinished() && !m_fadeUI->IsRunning()) {
		m_fadeUI->Start();
	}

	// UI�̃A���t�@�l���X�V����
	for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetAlpha(m_fadeUI->GetAlpha());
	}

	// UI�̃t�F�[�h�����������^�C�~���O�ŃJ�[�\���������ʒu�ɂ���
	if (!m_fadeUIFinished && m_fadeUI->IsFinished()) {
		m_menuUIs[m_selectedUI]->Select();
		m_fadeUIFinished = true;
	}

	// ���I���Ńt�F�[�h���������Ă�����UI���X�V����
	if (!m_wasSelected && m_fadeUI->IsFinished()) {
		// �p�b�h�ڑ����ɃJ�[�\���ړ����s��
		const InputManager* input_manager = ServiceLocater<InputManager>::Get();
		if (input_manager->IsPadConnected()) {
			if (input_manager->IsPressed(InputID::Left)) {
				m_menuUIs[m_selectedUI]->Deselect();
				m_selectedUI = (m_selectedUI + m_menuUIs.size() - 1) % m_menuUIs.size();
				m_menuUIs[m_selectedUI]->Select();
			}
			else if (input_manager->IsPressed(InputID::Right)) {
				m_menuUIs[m_selectedUI]->Deselect();
				m_selectedUI = (m_selectedUI + 1) % m_menuUIs.size();
				m_menuUIs[m_selectedUI]->Select();
			}
		}

		for (std::vector<std::unique_ptr<MenuUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}

		// �C�x���g���擾���Ă��邩�ǂ����m�F����
		if (m_uiObserver->HasNewEvent()) {
			UIEventID event_id = m_uiObserver->GetEventID();
			// �C�x���g�ɉ����ăV�[����؂�ւ���
			switch (event_id) {
				// �L�����Z���N�g�ɖ߂�
			case UIEventID::CharaSelect:
				m_nextSceneID = SceneID::CharaSelect;
				m_fadeScreen->Start();
				m_wasSelected = true;
				break;
				// �^�C�g���ɖ߂�
			case UIEventID::Title:
				m_nextSceneID = SceneID::Title;
				m_fadeScreen->Start();
				m_wasSelected = true;
				break;
			default:
				ErrorMessage(L"�s����UI�C�x���g���擾���܂���");
				break;
			}
		}
	}

	// �t�F�[�h�A�E�g������������V�[���J�ڂ���
	if (m_fadeScreen->IsFinished()) {
		m_pSceneRequest->RequestScene(m_nextSceneID);
	}

}

/// <summary>
/// �������U���g�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void WinResultScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// �������S��`�悷��
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Player1Win);
	DirectX::SimpleMath::Vector2 pos(ServiceLocater<DirectX11>::Get()->GetWidth()*0.5f, 100.0f);
	spriteBatch->Draw(texture->GetResource().Get(), pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, m_fadeLogo->GetAlpha()), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2::One);

	// UI��`�悷��
	for (std::vector<std::unique_ptr<MenuUI>>::const_iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}


	// ���Ԍo�߂Ńt�F�[�h�A�E�g���s��
	m_fadeScreen->Render(spriteBatch);

	spriteBatch->End();
}

/// <summary>
/// �������U���g�V�[�����I������
/// </summary>
void WinResultScene::Finalize() {

}

/// <summary>
/// UI������������
/// </summary>
void WinResultScene::InitializeUI() {
	// �I�u�U�[�o�𐶐�����
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UI�̐���
	// �L�����Z���N�g
	{
		std::unique_ptr<MenuUI> charaselect = std::make_unique<MenuUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.3f, screen_size.y*0.8f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// �^�C�g�� 
	{
		std::unique_ptr<MenuUI> title = std::make_unique<MenuUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.7f, screen_size.y*0.8f));
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

	m_selectedUI = 0;
}

