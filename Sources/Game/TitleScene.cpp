#include "TitleScene.h"
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

	InitializeUI();
}

/// <summary>
/// �^�C�g���V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// �G�X�P�[�v�L�[�ŏI��
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		ExitGame();
	}

	// �X�y�[�X�L�[�����������N���b�N�����ꍇ
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space) ||
		ServiceLocater<MouseWrapper>::Get()->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED ) {
		// �^�C�g�����\������Ă��Ȃ���΁A�^�C�}�[��i�߂ĕ\��������
		if (m_time < 2.0f) {
			m_time = 2.0f;
		}
		
	}

	// UI���X�V����
	if (m_time > 2.5f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Update(timer);
		}
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
			m_pSceneRequest->RequestScene("CharaSelect");
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
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UI��`�悷��
	if (m_time > 2.5f) {
		for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
	}

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"TitleScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// �^�C�g���V�[�����I������
/// </summary>
void TitleScene::Finalize() {
	// UI����I�u�U�[�o���f�^�b�`����
	for (std::vector<std::unique_ptr<ScaleUpUI>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
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
	{
		std::unique_ptr<ScaleUpUI> tutorial = std::make_unique<ScaleUpUI>(
			UIEventID::Tutorial, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.4f));
		tutorial->SetText(L"Tutorial");
		m_menuUIs.emplace_back(std::move(tutorial));
	}
	// �v���C
	{
		std::unique_ptr<ScaleUpUI> play = std::make_unique<ScaleUpUI>(
			UIEventID::Play, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.55f));
		play->SetText(L"Play");
		m_menuUIs.emplace_back(std::move(play));
	}
	// �I�v�V����
	{
		std::unique_ptr<ScaleUpUI> option = std::make_unique<ScaleUpUI>(
			UIEventID::Option, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.7f));
		option->SetText(L"Option");
		m_menuUIs.emplace_back(std::move(option));
	}
	// �I��
	{
		std::unique_ptr<ScaleUpUI> exit = std::make_unique<ScaleUpUI>(
			UIEventID::Exit, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f, screen_size.y*0.85f));
		exit->SetText(L"Exit");
		m_menuUIs.emplace_back(std::move(exit));
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

