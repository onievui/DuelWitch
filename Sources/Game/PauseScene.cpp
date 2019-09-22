#include "PauseScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MouseWrapper.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\ScaleUpUI.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


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
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// �R�����X�e�[�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// �G�t�F�N�g�t�@�N�g���𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	// UI������������
	InitializeUI();

	// �}�E�X�J�[�\����\������
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}

/// <summary>
/// �|�[�Y�V�[�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void PauseScene::Update(const DX::StepTimer& timer) {
	timer;

	// �G�X�P�[�v�L�[�������āA�v���C�V�[�����ĊJ����
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		Resume();
		return;
	}

	// UI���X�V����
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEventID event_id = m_uiObserver->GetEventID();
		// �C�x���g�ɉ����ăV�[����؂�ւ���
		switch (event_id) {
		// �ĊJ����
		case UIEventID::Resume:
			Resume();
			return;
		// �L�����Z���N�g�ɖ߂�
		case UIEventID::CharaSelect:
			m_pSceneRequest->RequestScene("CharaSelect");
			break;
		// �^�C�g���ɖ߂�
		case UIEventID::Title:
			m_pSceneRequest->RequestScene("Title");
			break;
		default:
			ErrorMessage(L"�s����UI�C�x���g���擾���܂���");
			break;
		}
	}

}

/// <summary>
/// �|�[�Y�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void PauseScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	// �v���C��ʂɍ����d�˂ĈÂ�����
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::BlackBack);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Color(1, 1, 1, 0.8f));

	// UI��`�悷��
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
	

	spriteBatch->End();
}

/// <summary>
/// �|�[�Y�V�[�����I������
/// </summary>
void PauseScene::Finalize() {
	// UI����I�u�U�[�o���f�^�b�`����
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
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
		std::unique_ptr<ScaleUpUI> resume = std::make_unique<ScaleUpUI>(
			UIEventID::Resume, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.25f));
		resume->SetText(L"Resume");
		m_menuUIs.emplace_back(std::move(resume));
	}
	// �L�����Z���N�g
	{
		std::unique_ptr<ScaleUpUI> charaselect = std::make_unique<ScaleUpUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.5f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// �^�C�g�� 
	{
		std::unique_ptr<ScaleUpUI> title = std::make_unique<ScaleUpUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.75f));
		title->SetText(L"Title");
		m_menuUIs.emplace_back(std::move(title));
	}
	
	// ���ʂ̏���
	const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->SetFont(font);
		(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
		(*itr)->SetTexture(texture);
		(*itr)->FitTextureSize();
		// UI�ɃI�u�U�[�o���A�^�b�`����
		(*itr)->Attach(m_uiObserver.get());
	}
}

/// <summary>
/// �|�[�Y����������
/// </summary>
void PauseScene::Resume() {
	m_pSceneRequest->PopScene();
	// �}�E�X�J�[�\�����\���ɖ߂�
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_RELATIVE);
}

