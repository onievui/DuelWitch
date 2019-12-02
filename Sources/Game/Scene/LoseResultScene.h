#pragma once
#ifndef LOSE_RESULT_SCENE_DEFINED
#define LOSE_RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


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
	// ���U���g�V�[�����I������
	void Finalize() override;

private:
	// UI������������
	void InitializeUI();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;

	// �^�C�}�[
	float                                                    m_time;
	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

};


#endif // !LOSE_RESULT_SCENE_DEFINED
