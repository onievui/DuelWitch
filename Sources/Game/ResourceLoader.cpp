#include "ResourceLoader.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\ErrorMessage.h>

#include "Element.h"
#include "NormalMagic.h"
#include "FireMagic.h"
#include "FreezeMagic.h"
#include "ThunderMagic.h"
#include "ThunderStrikeMagic.h"


/// <summary>
/// ���\�[�X��ǂݍ���
/// </summary>
/// <param name="id">���\�[�X�ǂݍ���ID</param>
void ResourceLoader::Load(ResourceLoaderID id) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	ResourceManager<TextureResource>* textureResourceManager = ServiceLocater<ResourceManager<TextureResource>>::Get();
	ResourceManager<GeometricPrimitiveResource>* geometricPrimitiveResourceManager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
	ResourceManager<ModelResource>* modelResourceManager = ServiceLocater<ResourceManager<ModelResource>>::Get();
	//ResourceManager<FontResource>* fontResourceManager = ServiceLocater<ResourceManager<FontResource>>::Get();
	

	switch (id) {
	case ResourceLoaderID::PlayScene:
		// �e�N�X�`���̓ǂݍ���
		textureResourceManager->AddResource(TextureID::FireMagicIcon, TextureResource(L"Protected/element1.png"));
		textureResourceManager->AddResource(TextureID::FreezeMagicIcon, TextureResource(L"Protected/element2.png"));
		textureResourceManager->AddResource(TextureID::ThunderMagicIcon, TextureResource(L"Protected/element3.png"));
		// �W�I���g���b�N�v���~�e�B�u�̐���
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::Element, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCube(context, Element::ELEMENT_RADIUS*2)));
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::NormalMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, NormalMagic::NORMAL_MAGIC_RADIUS)));
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::FireMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCone(context, FireMagic::FIRE_MAGIC_RADIUS, FireMagic::FIRE_MAGIC_RADIUS*2)));
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::FreezeMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, FreezeMagic::FREEZE_MAGIC_RADIUS)));
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::ThunderMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateSphere(context, ThunderMagic::THUNDER_MAGIC_RADIUS)));
		geometricPrimitiveResourceManager->AddResource(GeometricPrimitiveID::ThunderStrikeMagic, GeometricPrimitiveResource(
			DirectX::GeometricPrimitive::CreateCylinder(context,
				ThunderStrikeMagic::THUNDER_STRIKE_MAGIC_LENGTH, ThunderStrikeMagic::THUNDER_STRIKE_MAGIC_RADIUS*2)));
		// ���f���̓ǂݍ���
		modelResourceManager->AddResource(ModelID::BloomModel, ModelResource(L"bloom.cmo", L"Protected"));
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
