#include "ResourceLoader.h"
#include <Framework\DirectX11.h>
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
	ResourceManager<ModelResource>* modelResourceManager = ServiceLocater<ResourceManager<ModelResource>>::Get();
	ResourceManager<FontResource>* fontResourceManager = ServiceLocater<ResourceManager<FontResource>>::Get();
	ResourceManager<VertexShaderResource>* vertexShaderResourceManager = ServiceLocater<ResourceManager<VertexShaderResource>>::Get();
	ResourceManager<GeometryShaderResource>* geometryShaderResourceManager = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get();
	ResourceManager<PixelShaderResource>* pixelShaderResourceManager = ServiceLocater<ResourceManager<PixelShaderResource>>::Get();
	

	switch (id) {
	// ���ʂ̃��\�[�X
	case ResourceLoaderID::Common:
		fontResourceManager->AddResource(FontID::Default, FontResource(L"Protected/Arial.spritefont"));
		break;
	// ���S�V�[��
	case ResourceLoaderID::LogoScene:
		textureResourceManager->AddResource(TextureID::Logo, TextureResource(L"Protected/creator_logo.png"));
		break;
	// �^�C�g���V�[��
	case ResourceLoaderID::TitleScene:
		textureResourceManager->AddResource(TextureID::Title,   TextureResource(L"Protected/title1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		break;
	// �L�����Z���N�g�V�[��
	case ResourceLoaderID::CharaSelectScene:
		textureResourceManager->AddResource(TextureID::CharaSelectBackGround, TextureResource(L"Protected/chara_select1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame,               TextureResource(L"Protected/title_ui_frame.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,             TextureResource(L"Protected/chara_icon1.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,                 L"Protected/chara_icon2.png");
		textureResourceManager->AddResource(TextureID::CharaIcon,                 L"Protected/chara_icon3.png");
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,     TextureResource(L"Protected/chara_select_marker1.png"));
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,	      L"Protected/chara_select_marker2.png");
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,	      L"Protected/chara_select_marker3.png");
		break;
	// �v���C�V�[��
	case ResourceLoaderID::PlayScene:
		// �e�N�X�`���̓ǂݍ���
		textureResourceManager->AddResource(TextureID::MagicIcon,       TextureResource(L"Protected/element1.png"));
		textureResourceManager->AddResource(TextureID::MagicIcon,		    L"Protected/element2.png");
		textureResourceManager->AddResource(TextureID::MagicIcon,           L"Protected/element3.png");
		textureResourceManager->AddResource(TextureID::MagicAiming,     TextureResource(L"Protected/magic_aiming.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,       TextureResource(L"Protected/chara_icon1.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,           L"Protected/chara_icon2.png");
		textureResourceManager->AddResource(TextureID::CharaIcon,           L"Protected/chara_icon3.png");
		textureResourceManager->AddResource(TextureID::HpBar,           TextureResource(L"Protected/hp_bar_green.png"));
		textureResourceManager->AddResource(TextureID::HpBar,               L"Protected/hp_bar_red.png");
		textureResourceManager->AddResource(TextureID::HpBar,               L"Protected/hp_bar_black.png");
		textureResourceManager->AddResource(TextureID::SpBar,           TextureResource(L"Protected/sp_bar_blue.png"));
		textureResourceManager->AddResource(TextureID::SpBar,               L"Protected/sp_bar_black.png");
		// �G�t�F�N�g�Ŏg���e�N�X�`���̓ǂݍ���
		textureResourceManager->AddResource(TextureID::Particle,                 TextureResource(L"Protected/particle.png"));
		textureResourceManager->AddResource(TextureID::FieldShield,              TextureResource(L"Protected/shock_wave.png"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect,          TextureResource(L"Protected/fire_magic_effect_a.jpg"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect,				L"Protected/fire_magic_effect_b.png");
		textureResourceManager->AddResource(TextureID::Ice,                      TextureResource(L"Protected/ice_pattern3.jpg"));
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect, TextureResource(L"Protected/thunder_strike11.png"));
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike12.png");
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike13.png");
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike14.png");
		// �|�[�Y�V�[���Ŏg���e�N�X�`�����ǂ݂���ł���
		textureResourceManager->AddResource(TextureID::Fade,    TextureResource(L"Protected/fade_texture1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		// ���U���g�V�[���Ŏg���e�N�X�`�����ǂ݂���ł���
		textureResourceManager->AddResource(TextureID::Player1Win, TextureResource(L"Protected/player1_win.png"));
		textureResourceManager->AddResource(TextureID::YouLose,    TextureResource(L"Protected/you_lose.png"));
		// �V�F�[�_�̓ǂݍ���
		vertexShaderResourceManager->AddResource(VertexShaderID::Default,          VertexShaderResource(L"DefaultVS.cso", VertexShaderID::Default));
		vertexShaderResourceManager->AddResource(VertexShaderID::Ice,              VertexShaderResource(L"IceVS.cso", VertexShaderID::Ice));
		geometryShaderResourceManager->AddResource(GeometryShaderID::Billboard,    GeometryShaderResource(L"BillboardGS.cso"));
		pixelShaderResourceManager->AddResource(PixelShaderID::Default,            PixelShaderResource(L"DefaultPS.cso"));
		pixelShaderResourceManager->AddResource(PixelShaderID::ThunderStrikeMagic, PixelShaderResource(L"ThunderStrikePS.cso"));
		// ���f���̓ǂݍ���
		modelResourceManager->AddResource(ModelID::Bloom,   ModelResource(L"bloom.cmo", L"Protected"));
		modelResourceManager->AddResource(ModelID::Skydome, ModelResource(L"skydome.cmo", L"Protected"));
		break;
	default:
		ErrorMessage(L"���\�[�X�̃��[�h�ŕs����ID���n����܂���");
		break;
	}
}

/// <summary>
/// ���\�[�X���J������
/// </summary>
/// <param name="id">���\�[�X�ǂݍ���ID</param>
void ResourceLoader::Release(ResourceLoaderID id) {
	switch (id) {
	case ResourceLoaderID::Common:
		ServiceLocater<ResourceManager<FontResource>>::Get()->Release();
		break;
	case ResourceLoaderID::LogoScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		break;
	case ResourceLoaderID::TitleScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		break;
	case ResourceLoaderID::CharaSelectScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		break;
	case ResourceLoaderID::PlayScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()->Release();
		ServiceLocater<ResourceManager<ModelResource>>::Get()->Release();
		break;
	default:
		ErrorMessage(L"���\�[�X�̉���ŕs����ID���n����܂���");
		break;
	}
	
}
