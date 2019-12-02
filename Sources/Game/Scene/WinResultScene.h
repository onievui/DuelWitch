#pragma once
#ifndef WIN_RESULT_SCENE_DEFINED
#define WIN_RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


/// <summary>
/// �������U���g�V�[���N���X
/// </summary>
class WinResultScene : public IScene {
public:
	// �R���X�g���N�^
	WinResultScene();
	// �f�X�g���N�^
	~WinResultScene();

public:
	// �������U���g�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �������U���g�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �������U���g�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �������U���g�V�[�����I������
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


#endif // !WIN_RESULT_SCENE_DEFINED
