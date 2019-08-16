#pragma once
#ifndef MYGAME_DEFINED
#define MYGAME_DEFINED


#include <Framework\Window.h>
#include <Framework\Game.h>


class SceneManager;
template <class T>
class ResourceManager;
class TextureResource;
class GeometricPrimitiveResource;
class FontResource;

/// <summary>
/// �Q�[�����[�v�N���X
/// </summary>
class MyGame : public Game {
public:
	// �R���X�g���N�^
	MyGame(int width, int height);
	// �f�X�g���N�^
	~MyGame();
	// �Q�[��������������
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
	template <class T>
	// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	void CreateAndRegister(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind) {
		manager = std::make_unique<ResourceManager<T>>(kind);
		ServiceLocater<ResourceManager<T>>::Register(manager.get());
	};

private:
	// ��
	int                                m_width;
	// ����
	int                                m_height;

	// �L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// �}�E�X
	std::unique_ptr<DirectX::Mouse>    m_mouse;

	// �V�[���}�l�[�W��
	std::unique_ptr<SceneManager>      m_sceneManager;

	// ���\�[�X�}�l�[�W��
	std::unique_ptr<ResourceManager<TextureResource>>              m_textureResourceManager;
	std::unique_ptr<ResourceManager<GeometricPrimitiveResource>>   m_geometricPrimitiveResourceManager;
	std::unique_ptr<ResourceManager<FontResource>>                 m_fontResourceManager;

};

#endif	// MYGAME_DEFINED