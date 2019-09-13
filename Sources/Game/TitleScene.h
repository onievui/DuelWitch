#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class ScaleUpUI;


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
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

	// �^�C�}�[
	float                                                    m_time;
	// UI�I�u�U�[�o
	std::unique_ptr<UIObserver>                              m_uiObserver;
	// ���j���[UI
	std::vector<std::unique_ptr<ScaleUpUI>>                  m_menuUIs;

};


#endif // !TITLE_SCENE_DEFINED


