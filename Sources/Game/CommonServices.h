#pragma once
#ifndef COMMON_SERVICES_DEFINED
#define COMMON_SERVICES_DEFINED


#include <Utils\ServiceLocater.h>


template <class T>
class ResourceManager;
class TextureResource;
class GeometricPrimitiveResource;
class ModelResource;
class FontResource;


/// <summary>
/// ��{�I�ȃT�[�r�X���܂Ƃ߂��N���X
/// </summary>
class CommonServices {
public:
	// �R�����T�[�r�X������������
	void Initialize();
	// �I���������s��
	void Finalize();

private:
	template<class T, class... Args>
	// �T�[�r�X�𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	void RegisterService(std::unique_ptr<T>& service, Args&&... args) {
		service = std::make_unique<T>(args...);
		ServiceLocater<T>::Register(service.get());
	}

	template<class T>
	// ���\�[�X�}�l�[�W���𐶐����ăT�[�r�X���P�[�^�ɓo�^����
	void RegisterResourceManager(std::unique_ptr<ResourceManager<T>>& manager, const std::wstring& kind) {
		manager = std::make_unique<ResourceManager<T>>(kind);
		ServiceLocater<ResourceManager<T>>::Register(manager.get());
	};

private:
	// �g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>       m_keyboardStateTracker;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker>            m_mouseStateTracker;

	// ���\�[�X�}�l�[�W��
	std::unique_ptr<ResourceManager<TextureResource>>              m_textureResourceManager;
	std::unique_ptr<ResourceManager<GeometricPrimitiveResource>>   m_geometricPrimitiveResourceManager;
	std::unique_ptr<ResourceManager<ModelResource>>                m_modelResourceManager;
	std::unique_ptr<ResourceManager<FontResource>>                 m_fontResourceManager;
};


#endif // !COMMON_SERVICES_DEFINED
