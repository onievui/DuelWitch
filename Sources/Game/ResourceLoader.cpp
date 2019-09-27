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
/// リソースを読み込む
/// </summary>
/// <param name="id">リソース読み込みID</param>
void ResourceLoader::Load(ResourceLoaderID id) {
	//ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	ResourceManager<TextureResource>* textureResourceManager = ServiceLocater<ResourceManager<TextureResource>>::Get();
	//ResourceManager<GeometricPrimitiveResource>* geometricPrimitiveResourceManager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
	ResourceManager<ModelResource>* modelResourceManager = ServiceLocater<ResourceManager<ModelResource>>::Get();
	ResourceManager<FontResource>* fontResourceManager = ServiceLocater<ResourceManager<FontResource>>::Get();
	ResourceManager<VertexShaderResource>* vertexShaderResourceManager = ServiceLocater<ResourceManager<VertexShaderResource>>::Get();
	ResourceManager<GeometryShaderResource>* geometryShaderResourceManager = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get();
	ResourceManager<PixelShaderResource>* pixelShaderResourceManager = ServiceLocater<ResourceManager<PixelShaderResource>>::Get();
	

	switch (id) {
	// 共通のリソース
	case ResourceLoaderID::Common:
		fontResourceManager->AddResource(FontID::Default, FontResource(L"Protected/Arial.spritefont"));
		break;
	// ロゴシーン
	case ResourceLoaderID::LogoScene:
		textureResourceManager->AddResource(TextureID::Logo, TextureResource(L"Protected/creator_logo.png"));
		break;
	// タイトルシーン
	case ResourceLoaderID::TitleScene:
		textureResourceManager->AddResource(TextureID::Title, TextureResource(L"Protected/title1024.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		break;
	// キャラセレクトシーン
	case ResourceLoaderID::CharaSelectScene:
		textureResourceManager->AddResource(TextureID::CharaSelectBackGround, TextureResource(L"Protected/chara_select1024.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon, TextureResource(L"Protected/chara_icon1.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon, L"Protected/chara_icon2.png");
		textureResourceManager->AddResource(TextureID::CharaIcon, L"Protected/chara_icon3.png");
		textureResourceManager->AddResource(TextureID::CharaSelectMarker, TextureResource(L"Protected/chara_select_marker1.png"));
		textureResourceManager->AddResource(TextureID::CharaSelectMarker, L"Protected/chara_select_marker2.png");
		break;
	// プレイシーン
	case ResourceLoaderID::PlayScene:
		// テクスチャの読み込み
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
		textureResourceManager->AddResource(TextureID::Particle,        TextureResource(L"Protected/particle.png"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect, TextureResource(L"Protected/fire_magic_effect_a.jpg"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect,     L"Protected/fire_magic_effect_b.png");
		// ポーズシーンで使うテクスチャも読みこんでおく
		textureResourceManager->AddResource(TextureID::BlackBack, TextureResource(L"Protected/black_back1024.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		// シェーダの読み込み
		vertexShaderResourceManager->AddResource(VertexShaderID::Default, VertexShaderResource(L"DefaultVS.cso", VertexShaderID::Default));
		geometryShaderResourceManager->AddResource(GeometryShaderID::Billboard, GeometryShaderResource(L"BillboardGS.cso"));
		pixelShaderResourceManager->AddResource(PixelShaderID::Default, PixelShaderResource(L"DefaultPS.cso"));
		// モデルの読み込み
		modelResourceManager->AddResource(ModelID::BloomModel, ModelResource(L"bloom.cmo", L"Protected"));
		break;
	default:
		ErrorMessage(L"リソースのロードで不正なIDが渡されました");
		break;
	}
}

/// <summary>
/// リソースを開放する
/// </summary>
/// <param name="id">リソース読み込みID</param>
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
		ErrorMessage(L"リソースの解放で不正なIDが渡されました");
		break;
	}
	
}
