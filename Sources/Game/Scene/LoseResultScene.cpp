#include "LoseResultScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\ScaleUpUI.h>
#include <Utils\MouseWrapper.h>
#include "ISceneRequest.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
LoseResultScene::LoseResultScene() {

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LoseResultScene::~LoseResultScene() {
}

/// <summary>
///	�s�k���U���g�V�[��������������
/// </summary>
/// <param name="pSceneRequest"></param>
void LoseResultScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;

	m_time = 0.0f;

	// UI������������
	InitializeUI();

	// �}�E�X�J�[�\����\������
	ServiceLocater<MouseWrapper>::Get()->SetMode(DirectX::Mouse::Mode::MODE_ABSOLUTE);
}

/// <summary>
/// �s�k���U���g�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void LoseResultScene::Update(const DX::StepTimer& timer) {
	float elapesd_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapesd_time;

	// UI���X�V����
	if (m_time >= 3.0f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}
	}

	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEventID event_id = m_uiObserver->GetEventID();
		// �C�x���g�ɉ����ăV�[����؂�ւ���
		switch (event_id) {
			// �L�����Z���N�g�ɖ߂�
		case UIEventID::CharaSelect:
			m_pSceneRequest->RequestScene(SceneID::CharaSelect);
			break;
			// �^�C�g���ɖ߂�
		case UIEventID::Title:
			m_pSceneRequest->RequestScene(SceneID::Title);
			break;
		default:
			ErrorMessage(L"�s����UI�C�x���g���擾���܂���");
			break;
		}
	}
}

/// <summary>
/// �s�k���U���g�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void LoseResultScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

	// ���Ԍo�߂Ńt�F�[�h�A�E�g���s��
	float alpha;
	if (m_time <= 3.0f) {
		alpha = m_time / 3.0f*0.8f;
	}
	else {
		alpha = 0.8f;
	}

	// �v���C��ʂɍ����d�˂ĈÂ�����
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Fade);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Color(0, 0, 0, alpha));

	// �s�k�e�N�X�`����`�悷��
	if (m_time >= 3.0f) {		
		texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::YouLose);
		// ���Ԍo�߂Ńt�F�[�h�C�����s��
		alpha = std::min((m_time - 3.0f) / 4.0f, 1.0f);
		DirectX::SimpleMath::Vector2 pos(ServiceLocater<DirectX11>::Get()->GetWidth()*0.5f, 100.0f);
		spriteBatch->Draw(texture->GetResource().Get(), pos, nullptr, DirectX::SimpleMath::Color(1, 1, 1, alpha), 0,
			texture->GetCenter(), DirectX::SimpleMath::Vector2::One);
	}

	// UI��`�悷��
	if (m_time >= 4.0f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::const_iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}


	spriteBatch->End();
}

/// <summary>
/// ���U���g�V�[�����I������
/// </summary>
void LoseResultScene::Finalize() {

}

/// <summary>
/// UI������������
/// </summary>
void LoseResultScene::InitializeUI() {
	// �I�u�U�[�o�𐶐�����
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UI�̐���
	// �L�����Z���N�g
	{
		std::unique_ptr<ScaleUpUI> charaselect = std::make_unique<ScaleUpUI>(
			UIEventID::CharaSelect, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.3f, screen_size.y*0.8f));
		charaselect->SetText(L"CharaSelect");
		m_menuUIs.emplace_back(std::move(charaselect));
	}
	// �^�C�g�� 
	{
		std::unique_ptr<ScaleUpUI> title = std::make_unique<ScaleUpUI>(
			UIEventID::Title, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.7f, screen_size.y*0.8f));
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

