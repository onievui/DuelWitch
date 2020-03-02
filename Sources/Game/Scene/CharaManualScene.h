#pragma once
#ifndef CHARA_MANUAL_SCENE_DEFINED
#define CHARA_MANUAL_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class UIObserver;
class SoundScaleUpUI;
class Fade;
enum class SceneID;

/// <summary>
/// �L�����}�j���A���V�[���N���X
/// </summary>
class CharaManualScene : public IScene {
public:
	// �L�����}�j���A���̕\���ɂ����鎞��
	static constexpr float CHARA_MANUAL_TIME = 0.2f;

public:
	// �R���X�g���N�^
	CharaManualScene();
	// �f�X�g���N�^
	~CharaManualScene();

public:
	// �L�����}�j���A���V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �L�����}�j���A���V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �L�����}�j���A���V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �L�����}�j���A���V�[�����I������
	void Finalize() override;

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                                     m_pSceneRequest;

	// ��ʗp�t�F�[�h
	std::unique_ptr<Fade>                              m_fade;
	// �e�N�X�`���p�t�F�[�h
	std::unique_ptr<Fade>                              m_fadeTexture;
	// �}�j���A���̕\���������������ǂ���
	bool                                               m_canceled;
};


#endif // !CHARA_MANUAL_SCENE_DEFINED


