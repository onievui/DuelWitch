#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class PlayParameterLoader;
class DebugCamera;
class GridFloor;
class Player;
class PlayerManager;
class Element;
class ElementManager;
class IMagic;
class MagicManager;
class EffectManager;
class TargetCamera;
class Field;
class CollisionManager;
class Fade;
enum class SceneID;


/// <summary>
/// �v���C�V�[���N���X
/// </summary>
class PlayScene : public IScene {
public:
	// �R���X�g���N�^
	PlayScene();
	// �f�X�g���N�^
	~PlayScene();
public:
	// �v���C�V�[��������������
	void Initialize(ISceneRequest* pSceneRequest) override;
	// �v���C�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �v���C�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �v���C�V�[�����I������
	void Finalize() override;

private:
	// �����蔻����s��
	void DetectCollision();

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                           m_pSceneRequest;

	// ��ʗp�t�F�[�h
	std::unique_ptr<Fade>                    m_fade;
	// ���s���������ǂ���
	bool                                     m_isFinished;
	// �J�ڐ�̃V�[��ID
	SceneID                                  m_nextSceneID;

	// �p�����[�^�ǂݍ��݃N���X
	std::unique_ptr<PlayParameterLoader>     m_parameterLoader;

	// �f�o�b�O�J����
	std::unique_ptr<DebugCamera>             m_debugCamera;
	// �^�[�Q�b�g�J����
	std::unique_ptr<TargetCamera>            m_targetCamera;
	// �O���b�h��
	std::unique_ptr<GridFloor>               m_gridFloor;
	// �t�B�[���h
	std::unique_ptr<Field>                   m_field;
	// �v���C���[�}�l�[�W��
	std::unique_ptr<PlayerManager>           m_playerManager;
	// �G�������g�}�l�[�W��
	std::unique_ptr<ElementManager>          m_elementManager;
	// ���@�}�l�[�W��
	std::unique_ptr<MagicManager>            m_magicManager;
	// �G�t�F�N�g�}�l�[�W��
	std::unique_ptr<EffectManager>           m_effectManager;

	// �����蔻�菈���N���X
	std::unique_ptr<CollisionManager>        m_collisionManager;
	
};


#endif // !PLAY_SCENE_DEFINED
