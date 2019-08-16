#include "ResourceLoader.h"
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\ErrorMessage.h>


/// <summary>
/// リソースを読み込む
/// </summary>
/// <param name="id">リソース読み込みID</param>
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
		ErrorMessage(L"リソースのロードで不正なIDが渡されました");
		break;
	}
}

/// <summary>
/// リソースを開放する
/// </summary>
void ResourceLoader::Release() {
	ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
	ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()->Release();
	ServiceLocater<ResourceManager<FontResource>>::Get()->Release();
}
