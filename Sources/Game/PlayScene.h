#pragma once
#ifndef PLAY_SCENE_DEFINED
#define PLAY_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;
class DebugCamera;
class GridFloor;
class Player;
class ElementManager;
class TargetCamera;
class Field;

/// <summary>
/// �v���C�V�[���N���X
/// </summary>
class PlayScene : public IScene {
public:
	// �v���C�V�[��������������
	void Initialize(ISceneRequest* sceneRequest) override;
	// �v���C�V�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �v���C�V�[����`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// �v���C�V�[�����I������
	void Finalize() override;

private:
	// ���N�G�X�g�V�[���C���^�t�F�[�X
	ISceneRequest* m_sceneRequest;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_font;
};


#endif // !PLAY_SCENE_DEFINED


