#include "CharaSelectScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>
#include <Utils\RandMt.h>
#include "ISceneRequest.h"
#include <Game\Load\ResourceLoader.h>
#include <Game\UI\CharaIcon.h>
#include <Game\UI\CharaSelectMarker.h>
#include <Game\Scene\ShareData\ShareData.h>


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

	// ���\�[�X�̃��[�h
	ResourceLoader::Load(ResourceLoaderID::CharaSelectScene);

	m_time = 0.0f;
	m_state = CharaSelectState::SelectPlayer;
	m_currentPlayer = 0;
	m_selectCharaId.resize(PLAYER_COUNT);
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
	case CharaSelectScene::CharaSelectState::SelectPlayer:
		UpdateSelectPlayer(timer);
		break;
	case CharaSelectScene::CharaSelectState::Ready:
		UpdateReady(timer);
		break;
	default:
		ErrorMessage(L"�L�����Z���N�g�ŕs���ȏ�Ԃ��n����܂���");
		break;
	}

}

void CharaSelectScene::UpdateSelectPlayer(const DX::StepTimer& timer) {
	timer;

	// �����_���p�ϐ�
	int rand;
	// ���ɐi�񂾂��ǂ���
	bool select_next = false;

	// �C�x���g���擾���Ă��邩�ǂ����m�F����
	if (m_uiObserver->HasNewEvent()) {
		UIEvent ui_event = m_uiObserver->GetEvent();
		switch (ui_event.eventID) {
			
		case UIEventID::Back:
			// �^�C�g���ɖ߂�
			if (m_currentPlayer == 0) {
				m_pSceneRequest->RequestScene(SceneID::Title);
			}
			// �O�̃L�����N�^�[�̑I���ɖ߂�
			else {
				// �I���ς݃L�����N�^�[���B��
				m_backCharas[m_currentPlayer - 1]->SetTexture(nullptr);
				// ���삷��}�[�J�[��؂�ւ���
				m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
				m_markerUIs[m_currentPlayer - 1]->SetEnableObserver(true);
				--m_currentPlayer;
			}
			break;
		case UIEventID::Next:
			// �����_���ŃL���������߂�
			rand = RandMt::GetRand(static_cast<int>(m_charaIcons.size()));
			SelectChara(m_charaIcons[rand].get(), m_backCharas[m_currentPlayer].get(), m_markerUIs[m_currentPlayer].get());
			select_next = true;
			break;
			// �L�����N�^�[��I������
		case UIEventID::CharaIcon:
			SelectChara(ui_event.address, m_backCharas[m_currentPlayer].get(), m_markerUIs[m_currentPlayer].get());
			select_next = true;
			break;
		default:
			break;
		}
	}

	// ���ɐi�񂾏ꍇ
	if (select_next) {
		// ���̃L�����N�^�[�I���ɐi��
		if (m_currentPlayer != PLAYER_COUNT - 1) {
			// ���삷��}�[�J�[��؂�ւ���
			m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
			m_markerUIs[m_currentPlayer + 1]->SetEnableObserver(true);
			++m_currentPlayer;
		}
		// �Ō�̃L�����N�^�[��I�������ꍇ
		else {
			// �}�[�J�[�̑������������
			m_markerUIs[m_currentPlayer]->SetEnableObserver(false);
			// ���ɐi�ރ{�^���̃e�L�X�g��ύX����
			m_menuUIs[1]->SetText(L"Fight!");
			// ����҂���Ԃɐi��
			m_state = CharaSelectState::Ready;
		}
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
			// �L�����I���ɖ߂�
		case UIEventID::Back:
			m_backCharas[2]->SetTexture(nullptr);
			m_markerUIs[m_currentPlayer]->SetEnableObserver(true);
			m_state = CharaSelectState::SelectPlayer;
			break;
			// �L�������m�肷��
		case UIEventID::Next:
			// �I�������L������ID��ێ�����
			ServiceLocater<ShareData>::Get()->SetSelectCharaID(m_selectCharaId);
			m_pSceneRequest->RequestScene(SceneID::Play);
			break;
		default:
			break;
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
	// �I�������L������ID��ۑ�����
	m_selectCharaId[m_currentPlayer] = charaIcon->GetTextureIndex();
}


/// <summary>
/// �L�����Z���N�g�V�[����`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void CharaSelectScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied());

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
		for (int i = 0; i <= m_currentPlayer; ++i) {
			m_markerUIs[i]->Render(spriteBatch);
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
		// �L�����N�^�[�̐�
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
		for (int i = 0; i < PLAYER_COUNT; ++i) {
			std::unique_ptr<UISubject> chara = std::make_unique<UISubject>(
				UIEventID::Null, 0, DirectX::SimpleMath::Vector2(screen_size.x*(0.2f+0.3f*i), screen_size.y*0.4f));
			m_backCharas.emplace_back(std::move(chara));
		}
	}
	// �I���}�[�J�[
	{
		constexpr float scale = 0.6f;
		const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::CharaSelectMarker);
		for (int i = 0; i < PLAYER_COUNT; ++i) {
			std::unique_ptr<CharaSelectMarker> marker = std::make_unique<CharaSelectMarker>(
				0, DirectX::SimpleMath::Vector2::Zero, texture, i);
			if (i == 0) {
				// �ŏ��̃}�[�J�[�������ɔz�u����
				marker->SetPos(m_charaIcons.front()->GetPos());
			}
			else {
				marker->SetPos(m_charaIcons.front()->GetPos());
				// ���̃}�[�J�[�̈ʒu���Œ肷��
				marker->SetEnableObserver(false);
			}
			marker->SetScale(DirectX::SimpleMath::Vector2::One*scale);
			m_markerUIs.emplace_back(std::move(marker));

			// �L�����A�C�R���Ƀ}�[�J�[���A�^�b�`����
			for (std::vector<std::unique_ptr<CharaIcon>>::iterator itr = m_charaIcons.begin(); itr != m_charaIcons.end(); ++itr) {
				(*itr)->Attach(m_markerUIs[i].get());
			}
		}
	}

}


