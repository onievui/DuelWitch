#pragma once
#ifndef PAUSE_SCENE_DEFINED
#define PAUSE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


/// <summary>
/// �|�[�Y�V�[���N���X
/// </summary>
class PauseScene : public IScene {
public:
	// �R���X�g���N�^
	PauseScene();
	// �f�X�g���N�^
	~PauseScene();

public:
	// �|�[�Y�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �|�[�Y�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �|�[�Y�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �|�[�Y�V�[�����I������
	void Finalize() override;

private:
	// UI������������
	void InitializeUI();
	// �|�[�Y����������
	void Resume();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

};


#endif // !PAUSE_SCENE_DEFINED


