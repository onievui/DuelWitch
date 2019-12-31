#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;
class Fade;
enum class SceneID;

/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
class TitleScene : public IScene {
public:
	// �R���X�g���N�^
	TitleScene();
	// �f�X�g���N�^
	~TitleScene();

public:
	// �^�C�g���V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �^�C�g���V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �^�C�g���V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �^�C�g���V�[�����I������
	void Finalize() override;

private:
	// UI������������
	void InitializeUI();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;

	// ��ʗp�t�F�[�h
	std::unique_ptr<Fade>                                    m_fadeScreen;
	// ��ʗp�t�F�[�h�̒i�K
	int                                                      m_fadeScreenStep;
	// UI�p�t�F�[�h
	std::unique_ptr<Fade>                                    m_fadeUI;
	// UI�p�t�F�[�h�̒i�K
	int                                                      m_fadeUIStep;

	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

	// �I���ς݂��ǂ���
	bool                                                     m_wasSelected;
	// �J�ڐ�̃V�[��ID
	SceneID                                                  m_nextSceneID;
};


#endif // !TITLE_SCENE_DEFINED


