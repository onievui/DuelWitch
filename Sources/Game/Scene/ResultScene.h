#pragma once
#ifndef RESULT_SCENE_DEFINED
#define RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// ���U���g�V�[���N���X
/// </summary>
class ResultScene : public IScene {
public:
	// �R���X�g���N�^
	ResultScene();
	// �f�X�g���N�^
	~ResultScene();
public:
	// ���U���g�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// ���U���g�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// ���U���g�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// ���U���g�V�[�����I������
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


#endif // !RESULT_SCENE_DEFINED


