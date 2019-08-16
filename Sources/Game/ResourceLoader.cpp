#include "ResourceLoader.h"
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\ErrorMessage.h>


/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
/// <param name="id">���\�[�X�ǂݍ���ID</param>
void ResourceLoader::Load(ResourceLoaderID id) {
	ResourceManager<TextureResource>* textureResourceManager = ServiceLocater<ResourceManager<TextureResource>>::Get();
	//ResourceManager<GeometricPrimitiveResource>* geometricPrimitiveResourceManager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
	//ResourceManager<FontResource>* fontResourceManager = ServiceLocater<ResourceManager<FontResource>>::Get();
	
	switch (id) {
	case ResourceLoaderID::PlayScene:
		textureResourceManager->AddResource(TextureID::FireMagicIcon, TextureResource(L"Protected/element1.png"));
		textureResourceManager->AddResource(TextureID::FreezeMagicIcon, TextureResource(L"Protected/element2.png"));
		textureResourceManager->AddResource(TextureID::ThunderMagicIcon, TextureResource(L"Protected/element3.png"));
		break;
	default:
		ErrorMessage(L"���\�[�X�̃��[�h�ŕs����ID���n����܂���");
		break;
	}
}

/// <summary>
/// ���\�[�X���J������
/// </summary>
void ResourceLoader::Release() {
	ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()->Release();
	ServiceLocater<ResourceManager<FontResource>>::Get()->Release();
}
