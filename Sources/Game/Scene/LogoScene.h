#pragma once
#ifndef LOGO_SCENE_DEFINED
#define LOGO_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class Fade;


/// <summary>
/// ���S�V�[���N���X
/// </summary>
class LogoScene : public IScene {
public:
	// �R���X�g���N�^
	LogoScene();
	// �f�X�g���N�^
	~LogoScene();
public:
	// ���S�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// ���S�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// ���S�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// ���S�V�[�����I������
	void Finalize() override;

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                           m_pSceneRequest;

	// ��ʗp�t�F�[�h
	std::unique_ptr<Fade>                                    m_fade;
	// �t�F�[�h�̒i�K
	int                                                      m_fadeStep;
	
};


#endif // !LOGO_SCENE_DEFINED


