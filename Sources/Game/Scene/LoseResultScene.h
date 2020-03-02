#pragma once
#ifndef LOSE_RESULT_SCENE_DEFINED
#define LOSE_RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class MenuUI;
class Fade;
enum class SceneID;


/// <summary>
/// �s�k���U���g�V�[���N���X
/// </summary>
class LoseResultScene : public IScene {
public:
	// �R���X�g���N�^
	LoseResultScene();
	// �f�X�g���N�^
	~LoseResultScene();

public:
	// �s�k���U���g�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �s�k���U���g�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �s�k���U���g�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �s�k���U���g�V�[�����I������
	void Finalize() override;

private:
	// UI������������
	void InitializeUI();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;

	// ��ʗp�t�F�[�h
	std::unique_ptr<Fade>                                    m_fadeScreen;
	// ���U���g���S�p�t�F�[�h
	std::unique_ptr<Fade>                                    m_fadeLogo;
	// UI�p�t�F�[�h
	std::unique_ptr<Fade>                                    m_fadeUI;

	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<MenuUI>>             m_menuUIs;


	// �I�����ꂽ���ǂ���
	bool                                                     m_wasSelected;
	// �J�ڐ�̃V�[��ID
	SceneID                                                  m_nextSceneID;

};


#endif // !LOSE_RESULT_SCENE_DEFINED
