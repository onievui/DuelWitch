#pragma once
#ifndef CHARA_SELECT_SCENE_DEFINED
#define CHARA_SELECT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// �L�����Z���N�g�V�[���N���X
/// </summary>
class CharaSelectScene : public IScene {
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
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>                     m_font;
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

};


#endif // !CHARA_SELECT_SCENE_DEFINED


