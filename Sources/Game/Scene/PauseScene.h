#pragma once
#ifndef PAUSE_SCENE_DEFINED
#define PAUSE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class MenuUI;
class Fade;
enum class SceneID;

/// <summary>
/// �|�[�Y�V�[���N���X
/// </summary>
class PauseScene : public IScene {
public:
	// �|�[�Y�ɂ����鎞��
	static constexpr float PAUSE_TIME = 0.2f;
	// �|�[�Y����BGM����
	static constexpr float BGM_VOLUME = 0.3f;

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
	// �|�[�Y�̉�����I������
	void SelectResume();
	// �|�[�Y����������
	void Resume();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                     m_pSceneRequest;

	// ������ʗp�t�F�[�h
	std::unique_ptr<Fade>                              m_fadeBack;
	// ��O��ʗp�t�F�[�h
	std::unique_ptr<Fade>                              m_fadeFront;
	// UI�p�t�F�[�h
	std::unique_ptr<Fade>                              m_fadeUI;

	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                        m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<MenuUI>>               m_menuUIs;

	// �I�𒆂�UI
	int                                                m_selectedUI;
	// UI�̃t�F�[�h�������������ǂ���
	bool                                                     m_fadeUIFinished;
	// �I���ς݂��ǂ���
	bool                                               m_wasSelected;
	// �J�ڐ�̃V�[��ID
	SceneID                                            m_nextSceneID;
};


#endif // !PAUSE_SCENE_DEFINED


