#pragma once
#ifndef CHARA_SELECT_SCENE_DEFINED
#define CHARA_SELECT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class UISubject;
class CharaIcon;
class CharaSelectMarker;


/// <summary>
/// �L�����Z���N�g�V�[���N���X
/// </summary>
class CharaSelectScene : public IScene {
private:
	// �L�����Z���N�g�V�[���X�e�[�g
	enum class CharaSelectState {
		SelectPlayer,
		Ready,
	};

	// �I������v���C���[�̐�
	static constexpr int PLAYER_COUNT = 3;

public:
	// �R���X�g���N�^
	CharaSelectScene();
	// �f�X�g���N�^
	~CharaSelectScene();
public:
	// �L�����Z���N�g�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �L�����Z���N�g�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �L�����Z���N�g�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �L�����Z���N�g�V�[�����I������
	void Finalize() override;

private:
	// UI������������
	void InitializeUI();
	// �L�����N�^�[��I������
	void UpdateSelectPlayer(const DX::StepTimer& timer);
	// ����҂����
	void UpdateReady(const DX::StepTimer& timer);
	// �L������I������
	void SelectChara(const UISubject* charaIcon, UISubject* backChara, CharaSelectMarker* marker);

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                   m_pSceneRequest;
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory>         m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>          m_commonStates;

	// �^�C�}�[
	float                                            m_time;
	// �X�e�[�g
	CharaSelectState                                 m_state;
	// �I�𒆂̃v���C���[ID
	int                                              m_currentPlayer;
	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                      m_uiObserver;
	// �L�����A�C�R��UI
	std::vector<std::unique_ptr<CharaIcon>>          m_charaIcons;
	// �I���ς݃L�����N�^�[UI
	std::vector<std::unique_ptr<UISubject>>          m_backCharas;
	// �I���}�[�J�[UI
	std::vector<std::unique_ptr<CharaSelectMarker>>	 m_markerUIs;
	// ���j���[UI
	std::vector<std::unique_ptr<UISubject>>          m_menuUIs;
};


#endif // !CHARA_SELECT_SCENE_DEFINED


