#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED

#include <Framework\DirectX11.h>
#include <Framework\Window.h>
#include <Framework\Game.h>



class DebugCamera;
class GridFloor;
class Player;
class Element;
class TargetCamera;
class Field;

class MyGame : public Game {
public:
	// �R���X�g���N�^
	MyGame(int width, int height);
	// �f�X�g���N�^
	~MyGame();
	// �Q�[���I�u�W�F�N�g����������
	void Initialize(int width, int height) override;
	// ���\�[�X�𐶐�����
	void CreateResources() override;
	// �Q�[�����X�V����
	void Update(const DX::StepTimer& timer) override;
	// �Q�[����`�悷��
	void Render(const DX::StepTimer& timer) override;
	// �I�������������Ȃ�
	void Finalize() override;

	// FPS��`�悷��
	void DrawFPS(const DX::StepTimer& timer);

private:
	// ��
	int m_width;
	// ����
	int m_height;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;

	// �X�v���C�g�o�b�`
	DirectX::SpriteBatch* m_spriteBatch;
	// �G�t�F�N�g�t�@�N�g���C���^�[�t�F�[�X(m_fxFactory)
	std::unique_ptr<DirectX::IEffectFactory> m_effectFactory;
	// �R�����X�e�[�g
	std::unique_ptr <DirectX::CommonStates> m_commonStates;

	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// �}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;

	// �f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugCamera;
	// �^�[�Q�b�g�J����
	std::unique_ptr<TargetCamera> m_targetCamera;
	// �O���b�h��
	std::unique_ptr<GridFloor> m_gridFloor;
	
	// �t�B�[���h
	std::unique_ptr<Field> m_field;

	// ���f��
	std::unique_ptr<Player> m_model;
	// �G�������g
	std::unique_ptr<Element> m_element;

	// DirectX11�N���X�̃C���X�^���X���擾����
	DirectX11& m_directX = DirectX11::Get();
};

#endif	// MYGAME_DEFINED