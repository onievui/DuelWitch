#include "ResourceManager.h"
#include <Utils\ErrorMessage.h>


/// <summary>
/// コンストラクタ
/// </summary>
ResourceManager::ResourceManager() {
	Release();
	m_texturesIndex.resize(static_cast<int>(TextureID::Num), NULL_INDEX);
	m_fontsIndex.resize(static_cast<int>(FontID::Num), NULL_INDEX);
}

/// <summary>
/// デストラクタ
/// </summary>
ResourceManager::~ResourceManager() {
}

/// <summary>
/// リソースをロードする
/// </summary>
void ResourceManager::Load() {
	AddTexture(TextureID::FireMagicIcon,    std::make_shared<TextureResource>(L"Protected/element1.png"));
	AddTexture(TextureID::FreezeMagicIcon,  std::make_shared<TextureResource>(L"Protected/element2.png"));
	AddTexture(TextureID::ThunderMagicIcon, std::make_shared<TextureResource>(L"Protected/element3.png"));
}

/// <summary>
/// リソースを開放する
/// </summary>
void ResourceManager::Release() {
	Reset(m_textures, m_texturesIndex);
	Reset(m_fonts, m_fontsIndex);
}

/// <summary>
/// テクスチャリソースを取得する
/// </summary>
/// <param name="id">テクスチャID</param>
/// <returns>
/// テクスチャリソース
/// </returns>
std::shared_ptr<TextureResource> ResourceManager::GetTexture(TextureID id) {
	int index = m_texturesIndex[static_cast<int>(id)];
	if (index == NULL_INDEX) {
		ErrorMessage(L"画像の取得で不正な値が渡されました");
		return nullptr;
	}
	return m_textures[index];
}

/// <summary>
/// フォントリソースを取得する
/// </summary>
/// <param name="id">フォントID</param>
/// <returns>
/// フォントリソース
/// </returns>
std::shared_ptr<FontResource> ResourceManager::GetFont(FontID id) {
	int index = m_fontsIndex[static_cast<int>(id)];
	if (index == NULL_INDEX) {
		ErrorMessage(L"フォントの取得で不正な値が渡されました");
		return nullptr;
	}
	return m_fonts[index];
}

/// <summary>
/// テクスチャリソースを追加する
/// </summary>
/// <param name="id">テクスチャID</param>
/// <param name="texture">テクスチャリソース</param>
/// <returns>
/// true  : 追加に成功
/// false : 追加に失敗
/// </returns>
bool ResourceManager::AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture) {
	if (!CheckIsValid(texture)) {
		ErrorMessage(L"画像の読み込みに失敗しました");
		return false;
	}
	AddResource(texture, static_cast<int>(id), m_textures, m_texturesIndex);
	return true;
}

/// <summary>
/// フォントリソースを追加する
/// </summary>
/// <param name="id">フォントID</param>
/// <param name="font">フォントリソース</param>
/// <returns>
/// true  : 追加に成功
/// false : 追加に失敗
/// </returns>
bool ResourceManager::AddFont(FontID id, std::shared_ptr<FontResource>& font) {
	if (!CheckIsValid(font)) {
		ErrorMessage(L"フォントの読み込みに失敗しました");
		return false;
	}
	AddResource(font, static_cast<int>(id), m_fonts, m_fontsIndex);
	return true;
}

