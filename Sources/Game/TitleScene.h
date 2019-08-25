#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


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
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>                     m_font;
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

};


#endif // !TITLE_SCENE_DEFINED


