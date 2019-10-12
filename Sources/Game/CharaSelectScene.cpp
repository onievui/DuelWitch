#include "CharaSelectScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>
#include <Utils\RandMt.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"
#include "CharaIcon.h"
#include "CharaSelectMarker.h"


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

	// ���\�[�X�̃��[�h
	ResourceLoader::Load(ResourceLoaderID::CharaSelectScene);

	m_time = 0.0f;
	m_state = CharaSelectState::SelectPlayer1;
	// UI������������
	InitializeUI();
}

/// <summary>
/// �L�����Z���N�g�V�[�����X�V����
/// </summary>
/// <param name="timer"></param>
void CharaSelectScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// �G�X�P�[�v�L�[�ŏI��
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Escape)) {
		ExitGame();
	}

	// UI���X�V����
	// �߂�E�i�ރ{�^��
	for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
		(*itr)->Update(timer);
	}
	// �L�����A�C�R��
	for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
		(*itr)->Update(timer);
	}

	// �X�e�[�g�ɉ������X�V�������s��
	switch (m_state) {
	case CharaSelectScene::CharaSelectState::SelectPlayer1:
		UpdateSelectPlayer1(timer);
		break;
	case CharaSelectScene::CharaSelectState::SelectPlayer2:
		UpdateSelectPlayer2(timer);
		break;
	case CharaSelectScene::CharaSelectState::Ready:
		UpdateReady(timer);
		break;
	default:
		ErrorMessage(L"�L�����Z���N�g�ŕs���ȏ�Ԃ��n����܂���");
		break;
	}

}

/// <summary>
/// �v���C���[1�̃L�����N�^�[��I������
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CharaSelectScene::UpdateSelectPlayer1(const DX::StepTimer& timer) {
	timer;
	// �����_���p�ϐ�
	int rand;
	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			// �^�C�g���ɖ߂�
		case UIEventID::Back:
			m_pSceneRequest->RequestScene("Title");
			break;
			// �����_���ŃL���������߂�
		case UIEventID::Next:
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[0].get(), m_markerUIs[0].get());
			m_state = CharaSelectState::SelectPlayer2;
			break;
			// �L�����N�^�[��I������
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[0].get(), m_markerUIs[0].get());
			m_state = CharaSelectState::SelectPlayer2;
			break;
		default:
			break;
		}
	}

	// ���ɐi�ޏꍇ
	if (m_state == CharaSelectState::SelectPlayer2) {
		// �L�����A�C�R���ɃA�^�b�`����}�[�J�[��ύX����
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[0].get());
			(*itr)->Attach(m_markerUIs[1].get());
		}
	}
}

/// <summary>
/// �v���C���[2�̃L�����N�^�[��I������
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CharaSelectScene::UpdateSelectPlayer2(const DX::StepTimer& timer) {
	timer;
	// �����_���p�ϐ�
	int rand;
	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
		// �v���C���[1�̃L�����I���ɖ߂�
		case UIEventID::Back:
			m_backCharas[0]->SetTexture(nullptr);
			m_state = CharaSelectState::SelectPlayer1;
			break;
		// �����_���ŃL���������߂�
		case UIEventID::Next:
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[1].get(), m_markerUIs[1].get());
			m_state = CharaSelectState::Ready;
			break;
		// �L�����N�^�[��I������
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[1].get(), m_markerUIs[1].get());
			m_state = CharaSelectState::Ready;
			break;
		default:
			break;
		}
	}

	// �O�ɖ߂�ꍇ
	if (m_state == CharaSelectState::SelectPlayer1) {
		// �L�����A�C�R���ɃA�^�b�`����}�[�J�[��ύX����
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[1].get());
			(*itr)->Attach(m_markerUIs[0].get());
		}
	}
	// ���ɐi�ޏꍇ
	if (m_state == CharaSelectState::Ready) {
		// �L�����A�C�R���ɃA�^�b�`����}�[�J�[��ύX����
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Detach(m_markerUIs[1].get());
		}
		m_menuUIs[1]->SetText(L"Fight!");
	}
}

/// <summary>
/// ����҂����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void CharaSelectScene::UpdateReady(const DX::StepTimer& timer) {
	timer;
	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			// �v���C���[2�̃L�����I���ɖ߂�
		case UIEventID::Back:
			m_backCharas[1]->SetTexture(nullptr);
			m_state = CharaSelectState::SelectPlayer2;
			break;
			// �L�������m�肷��
		case UIEventID::Next:
			m_pSceneRequest->RequestScene("Play");
			break;
		default:
			break;
		}
	}

	// �O�ɖ߂�ꍇ
	if (m_state == CharaSelectState::SelectPlayer1) {
		// �L�����A�C�R���ɃA�^�b�`����}�[�J�[��ύX����
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Attach(m_markerUIs[1].get());
		}
	}
}

/// <summary>
/// �L������I������
/// </summary>
/// <param name="charaIcon">�L�����A�C�R��</param>
/// <param name="backChara">�I���ς݃L����</param>
/// <param name="marker">�I���}�[�J�[</param>
void CharaSelectScene::SelectChara(const UISubject* charaIcon, UISubject* backChara, CharaSelectMarker* marker) {
	// �I���ς݃L�����Ƀe�N�X�`�����Z�b�g����
	backChara->SetTexture(charaIcon->GetTexture());
	backChara->SetTextureIndex(charaIcon->GetTextureIndex());
	backChara->FitTextureSize();
	// �����_���őI�������ꍇ�̂��߂ɁA�}�[�J�[�𓮂���
	marker->SetPos(charaIcon->GetPos());
}


/// <summary>
/// �L�����Z���N�g�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void CharaSelectScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectBackGround);
	// ���Ԍo�߂Ńt�F�[�h�C�����s��
	float alpha = std::min(m_time, 1.0f);
	// �w�i��`�悷��
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	// UI��`�悷��
	if (m_time > 1.0f) {
		// �߂�E�i�ރ{�^��
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// �L�����A�C�R��
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// �I���ς݃L����
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_backCharas.begin(); itr != m_backCharas.end(); ++itr) {
			(*itr)->Render(spriteBatch);
		}
		// �I���}�[�J�[
		m_markerUIs[0]->Render(spriteBatch);
		if (m_state != CharaSelectState::SelectPlayer1) {
			m_markerUIs[1]->Render(spriteBatch);
		}
	}

	spriteBatch->End();
}

/// <summary>
/// �L�����Z���N�g�V�[�����I������
/// </summary>
void CharaSelectScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::CharaSelectScene);
}

/// <summary>
/// UI������������
/// </summary>
void CharaSelectScene::InitializeUI() {
	m_uiObserver = std::make_unique<UIObserver>();

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	DirectX::SimpleMath::Vector2 screen_size(static_cast<float>(directX->GetWidth()), static_cast<float>(directX->GetHeight()));

	// UI�̐���
	// �{�^��
	{
		// �߂�{�^��
		std::unique_ptr<UISubject> back = std::make_unique<UISubject>(
			UIEventID::Back, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.1f, screen_size.y*0.9f));
		back->SetText(L"Back");
		m_menuUIs.emplace_back(std::move(back));
		// �i�ރ{�^��
		std::unique_ptr<UISubject> next = std::make_unique<UISubject>(
			UIEventID::Next, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.9f, screen_size.y*0.9f));
		next->SetText(L"Random");
		m_menuUIs.emplace_back(std::move(next));
		// �{�^���̋��ʏ���
		const FontResource* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default);
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::UIFrame);
		for (std::vector<std::unique_ptr<UISubject>>::iterator itr = m_menuUIs.begin(); itr != m_menuUIs.end(); ++itr) {
			(*itr)->SetFont(font);
			(*itr)->SetTextColor(DirectX::SimpleMath::Color(DirectX::Colors::Black));
			(*itr)->SetTexture(texture);
			(*itr)->FitTextureSize();
			// UI�ɃI�u�U�[�o���A�^�b�`����
			(*itr)->Attach(m_uiObserver.get());
		}
	}

	// �L�����A�C�R���̐���
	{
		// �L�����A�C�R���̐�
		constexpr int chara_count = 3;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaIcon);
		float scale = 0.6f;
		float texture_width = texture->GetSize().x*scale;
		for (int i = 0; i < chara_count; ++i) {
			// �L�����A�C�R������ׂ邽�߂̃I�t�Z�b�g
			float x = (i - (chara_count - 1)*0.5f)*texture_width;
			std::unique_ptr<CharaIcon> chara_icon = std::make_unique<CharaIcon>(
				0, DirectX::SimpleMath::Vector2(screen_size.x*0.5f+x, screen_size.y*0.75f), texture, i);
			chara_icon->SetScale(DirectX::SimpleMath::Vector2::One*scale);
			chara_icon->Attach(m_uiObserver.get());
			m_charaIcons.emplace_back(std::move(chara_icon));
		}
	}
	// �I���ς݃L�����N�^�[
	{
		// �v���C���[1�L�����N�^�[
		std::unique_ptr<UISubject> chara1 = std::make_unique<UISubject>(
			UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.2f, screen_size.y*0.4f));
		m_backCharas.emplace_back(std::move(chara1));
		// �v���C���[2�L�����N�^�[
		std::unique_ptr<UISubject> chara2 = std::make_unique<UISubject>(
			UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*0.8f, screen_size.y*0.4f));
		m_backCharas.emplace_back(std::move(chara2));
	}
	// �I���}�[�J�[
	{
		float scale = 0.6f;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectMarker);
		// �v���C���[1�I���}�[�J�[
		std::unique_ptr<CharaSelectMarker> marker1 = std::make_unique<CharaSelectMarker>(
			0, m_charaIcons.front()->GetPos(), texture, 0);
		marker1->SetScale(DirectX::SimpleMath::Vector2::One*scale);
		m_markerUIs.emplace_back(std::move(marker1));
		// �v���C���[2�I���}�[�J�[
		std::unique_ptr<CharaSelectMarker> marker2 = std::make_unique<CharaSelectMarker>(
			0, m_charaIcons.back()->GetPos(), texture, 1);
		marker2->SetScale(DirectX::SimpleMath::Vector2::One*scale);
		m_markerUIs.emplace_back(std::move(marker2));

		// �L�����A�C�R���Ƀ}�[�J�[���A�^�b�`����
		for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
			(*itr)->Attach(m_markerUIs[0].get());
		}
	}

}

