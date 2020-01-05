#include "ResourceLoader.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\ErrorMessage.h>


/// <summary>
/// リソースを読み込む
/// </summary>
/// <param name="id">リソース読み込みID</param>
void ResourceLoader::Load(ResourceLoaderID id) {
	ResourceManager<TextureResource>* textureResourceManager = ServiceLocater<ResourceManager<TextureResource>>::Get();
	//ResourceManager<GeometricPrimitiveResource>* geometricPrimitiveResourceManager = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get();
	ResourceManager<ModelResource>* modelResourceManager = ServiceLocater<ResourceManager<ModelResource>>::Get();
	ResourceManager<SoundResource>* soundResourceManager = ServiceLocater<ResourceManager<SoundResource>>::Get();
	ResourceManager<BgmResource>* bgmResourceManager = ServiceLocater<ResourceManager<BgmResource>>::Get();
	ResourceManager<FontResource>* fontResourceManager = ServiceLocater<ResourceManager<FontResource>>::Get();
	ResourceManager<VertexShaderResource>* vertexShaderResourceManager = ServiceLocater<ResourceManager<VertexShaderResource>>::Get();
	ResourceManager<GeometryShaderResource>* geometryShaderResourceManager = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get();
	ResourceManager<PixelShaderResource>* pixelShaderResourceManager = ServiceLocater<ResourceManager<PixelShaderResource>>::Get();

	DirectX::AudioEngine* audioEngine = ServiceLocater<AudioManager>::Get()->GetAudioEngine();

	switch (id) {
	// 共通のリソース
	case ResourceLoaderID::Common:
		// フォントの読み込み
		fontResourceManager->AddResource(FontID::Default, FontResource(L"Protected/Arial.spritefont"));
		break;
	// ロゴシーン
	case ResourceLoaderID::LogoScene:
		// テクスチャの読み込み
		textureResourceManager->AddResource(TextureID::Logo, TextureResource(L"Protected/creator_logo.png"));
		textureResourceManager->AddResource(TextureID::Fade, TextureResource(L"Protected/fade_texture1280.png"));
		break;
	// タイトルシーン
	case ResourceLoaderID::TitleScene:
		// テクスチャの読み込み
		textureResourceManager->AddResource(TextureID::Title,   TextureResource(L"Protected/title1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		textureResourceManager->AddResource(TextureID::Fade,    TextureResource(L"Protected/fade_texture1280.png"));
		// サウンドの読み込み
		soundResourceManager->AddResource(SoundID::Cursor,   SoundResource(L"Protected/cursor.wav", audioEngine));
		soundResourceManager->AddResource(SoundID::Decision, SoundResource(L"Protected/decision.wav", audioEngine));
		break;
	// キャラセレクトシーン
	case ResourceLoaderID::CharaSelectScene:
		// テクスチャの読み込み
		textureResourceManager->AddResource(TextureID::CharaSelectBackGround, TextureResource(L"Protected/chara_select1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame,               TextureResource(L"Protected/title_ui_frame.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,             TextureResource(L"Protected/chara_icon1.png"));
		textureResourceManager->AddResource(TextureID::CharaIcon,                 L"Protected/chara_icon2.png");
		textureResourceManager->AddResource(TextureID::CharaIcon,                 L"Protected/chara_icon3.png");
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,     TextureResource(L"Protected/chara_select_marker1.png"));
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,	      L"Protected/chara_select_marker2.png");
		textureResourceManager->AddResource(TextureID::CharaSelectMarker,	      L"Protected/chara_select_marker3.png");
		textureResourceManager->AddResource(TextureID::Fade,                  TextureResource(L"Protected/fade_texture1280.png"));
		// サウンドの読み込み
		soundResourceManager->AddResource(SoundID::Cursor,   SoundResource(L"Protected/cursor.wav", audioEngine));
		soundResourceManager->AddResource(SoundID::Decision, SoundResource(L"Protected/decision.wav", audioEngine));
		soundResourceManager->AddResource(SoundID::Ready,    SoundResource(L"Protected/ready.wav", audioEngine));
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
		// エフェクトで使うテクスチャの読み込み
		textureResourceManager->AddResource(TextureID::Particle,                 TextureResource(L"Protected/particle.png"));
		textureResourceManager->AddResource(TextureID::PlayerCharge,             TextureResource(L"Protected/player_charge.png"));
		textureResourceManager->AddResource(TextureID::FieldShield,              TextureResource(L"Protected/field_shield_effect.png"));
		textureResourceManager->AddResource(TextureID::Hit,						 TextureResource(L"Protected/hit_effect3.png"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect,          TextureResource(L"Protected/fire_magic_effect_a.jpg"));
		textureResourceManager->AddResource(TextureID::FireMagicEffect,				L"Protected/fire_magic_effect_b.png");
		textureResourceManager->AddResource(TextureID::Ice,                      TextureResource(L"Protected/ice_pattern3.jpg"));
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect, TextureResource(L"Protected/thunder_strike11.png"));
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike12.png");
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike13.png");
		textureResourceManager->AddResource(TextureID::ThunderStrikeMagicEffect,	L"Protected/thunder_strike14.png");
		// ポーズシーンで使うテクスチャも読みこんでおく
		textureResourceManager->AddResource(TextureID::Fade,    TextureResource(L"Protected/fade_texture1280.png"));
		textureResourceManager->AddResource(TextureID::UIFrame, TextureResource(L"Protected/title_ui_frame.png"));
		// リザルトシーンで使うテクスチャも読みこんでおく
		textureResourceManager->AddResource(TextureID::Player1Win, TextureResource(L"Protected/player1_win.png"));
		textureResourceManager->AddResource(TextureID::YouLose,    TextureResource(L"Protected/you_lose.png"));
		// シェーダの読み込み
		vertexShaderResourceManager->AddResource(VertexShaderID::Default,          VertexShaderResource(L"DefaultVS.cso", VertexShaderID::Default));
		vertexShaderResourceManager->AddResource(VertexShaderID::Ice,              VertexShaderResource(L"IceVS.cso", VertexShaderID::Ice));
		geometryShaderResourceManager->AddResource(GeometryShaderID::Billboard,    GeometryShaderResource(L"BillboardGS.cso"));
		pixelShaderResourceManager->AddResource(PixelShaderID::Default,            PixelShaderResource(L"DefaultPS.cso"));
		pixelShaderResourceManager->AddResource(PixelShaderID::ThunderStrikeMagic, PixelShaderResource(L"ThunderStrikePS.cso"));
		// モデルの読み込み
		modelResourceManager->AddResource(ModelID::Chara,   ModelResource(L"unity-chan.cmo", L"Protected"));
		modelResourceManager->AddResource(ModelID::Bloom,   ModelResource(L"bloom.cmo", L"Protected"));
		modelResourceManager->AddResource(ModelID::Skydome, ModelResource(L"skydome.cmo", L"Protected"));
		// サウンドの読み込み
		soundResourceManager->AddResource(SoundID::Cursor,             SoundResource(L"Protected/cursor.wav",               audioEngine));
		soundResourceManager->AddResource(SoundID::Decision,           SoundResource(L"Protected/decision.wav",             audioEngine));
		soundResourceManager->AddResource(SoundID::Pause,              SoundResource(L"Protected/pause.wav",                audioEngine));
		soundResourceManager->AddResource(SoundID::NormalMagic,        SoundResource(L"Protected/normal_magic.wav",         audioEngine));
		soundResourceManager->AddResource(SoundID::FireMagic,          SoundResource(L"Protected/fire_magic.wav",           audioEngine));
		soundResourceManager->AddResource(SoundID::FreezeMagic,        SoundResource(L"Protected/freeze_magic.wav",         audioEngine));
		soundResourceManager->AddResource(SoundID::ThunderMagic,       SoundResource(L"Protected/thunder_magic.wav",        audioEngine));
		soundResourceManager->AddResource(SoundID::ThunderStrikeMagic, SoundResource(L"Protected/thunder_strike_magic.wav", audioEngine));
		soundResourceManager->AddResource(SoundID::Charge,             SoundResource(L"Protected/charge.wav",               audioEngine));
		soundResourceManager->AddResource(SoundID::Damage,             SoundResource(L"Protected/damage.wav",               audioEngine));
		soundResourceManager->AddResource(SoundID::Boost,              SoundResource(L"Protected/boost.wav",                audioEngine));
		soundResourceManager->AddResource(SoundID::Rolling,            SoundResource(L"Protected/rolling.wav",              audioEngine));
		soundResourceManager->AddResource(SoundID::GetElement,         SoundResource(L"Protected/get_element.wav",          audioEngine));
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
		ReleaseAudio();
		break;
	case ResourceLoaderID::TitleScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		ReleaseAudio();
		break;
	case ResourceLoaderID::CharaSelectScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		ReleaseAudio();
		break;
	case ResourceLoaderID::PlayScene:
		ServiceLocater<ResourceManager<TextureResource>>::Get()->Release();
		ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()->Release();
		ServiceLocater<ResourceManager<ModelResource>>::Get()->Release();
		ReleaseAudio();
		break;
	default:
		ErrorMessage(L"リソースの解放で不正なIDが渡されました");
		break;
	}
	
}

/// <summary>
/// 音声リソースを開放する
/// </summary>
void ResourceLoader::ReleaseAudio() {
	ResourceManager<SoundResource>* soundResourceManager = ServiceLocater<ResourceManager<SoundResource>>::Get();
	ResourceManager<BgmResource>* bgmResourceManager = ServiceLocater<ResourceManager<BgmResource>>::Get();
	AudioManager* audioManager = ServiceLocater<AudioManager>::Get();

	std::vector<std::unique_ptr<SoundResource>>& sounds = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawAllResources();
	for (std::vector<std::unique_ptr<SoundResource>>::iterator itr = sounds.begin(); itr != sounds.end(); ++itr) {
		int num = (*itr)->GetAllResources().size();
		for (int i = 0; i < num; ++i) {
			if (!(*itr)->IsValid(i)) {
				continue;
			}
			// 使用中の効果音を抽出してオーディオマネージャで管理する
			if ((*itr)->GetResource(i)->IsInUse()) {
				audioManager->RegisterUsingSoundEffect(std::move((*itr)->GetResource(i)));
			}
		}
	}

	std::vector<std::unique_ptr<BgmResource>>& bgms = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawAllResources();
	for (std::vector<std::unique_ptr<BgmResource>>::iterator itr = bgms.begin(); itr != bgms.end(); ++itr) {
		int num = (*itr)->GetAllResources().size();
		for (int i = 0; i < num; ++i) {
			if ((*itr)->IsValid(i)) {
				continue;
			}
			// 使用中のBGMを抽出してオーディオマネージャで管理する
			if ((*itr)->GetResource(i)->IsInUse()) {
				audioManager->RegisterUsingSoundEffect(std::move((*itr)->GetResource(i)));
			}
		}
	}

	soundResourceManager->Release();
	bgmResourceManager->Release();
}
