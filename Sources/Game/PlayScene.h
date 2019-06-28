#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class DebugCamera;
class GridFloor;
class Player;
class Element;
class ElementManager;
class IMagic;
class MagicManager;
class TargetCamera;
class Field;

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
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest*                           m_pSceneRequest;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>     m_font;
	// �G�t�F�N�g�t�@�N�g���C���^�t�F�[�X
	std::unique_ptr<DirectX::IEffectFactory> m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates>  m_commonStates;

	// �f�o�b�O�J����
	std::unique_ptr<DebugCamera>             m_debugCamera;
	// �^�[�Q�b�g�J����
	std::unique_ptr<TargetCamera>            m_targetCamera;
	// �O���b�h��
	std::unique_ptr<GridFloor>               m_gridFloor;

	// �t�B�[���h
	std::unique_ptr<Field>                   m_field;

	// ���f��
	std::unique_ptr<Player>                  m_player;
	// �G�������g�}�l�[�W��
	std::unique_ptr<ElementManager>          m_elementManager;
	// �G�������g
	std::vector<Element*>*                   m_pElements;
	// ���@�}�l�[�W��
	std::unique_ptr<MagicManager>            m_magicManager;
	// ���@
	std::vector<IMagic*>*                    m_pMagics;
	
};


#endif // !PLAY_SCENE_DEFINED


