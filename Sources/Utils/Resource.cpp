#include "Resource.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "ErrorMessage.h"


const std::wstring TextureResource::TEXTURE_DIR = L"Resources/Textures/";
const std::wstring FontResource::FONT_DIR       = L"Resources/Textures/";

TextureResource::Type Resource<TextureResource::Type>::m_defaultResource = nullptr;
FontResource::Type Resource<FontResource::Type>::m_defaultResource = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="filename">読み込む画像のファイル名</param>
TextureResource::TextureResource(const std::wstring& filename) {
	m_resources.emplace_back(nullptr);
	DirectX::CreateWICTextureFromFile(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(TEXTURE_DIR + filename).c_str(), nullptr, m_resources.back().GetAddressOf());
	if (m_resources.back().Get() == m_defaultResource.Get()) {
		ErrorMessage(L"画像の読み込みに失敗しました");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
TextureResource::~TextureResource() {
}

/// <summary>
/// リソースが有効かどうか確認する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// true  : 有効
/// false : 無効
/// </returns>
bool TextureResource::IsValid(int index) const {
	return m_resources[index].Get() != m_defaultResource.Get();
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="filename">読み込むフォントファイル</param>
FontResource::FontResource(const std::wstring& filename) {
	m_resources.emplace_back(
		std::make_unique<DirectX::SpriteFont>(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(FONT_DIR + filename).c_str())
	);
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"フォントの読み込みに失敗しました");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
FontResource::~FontResource() {
}

/// <summary>
/// リソースが有効かどうか確認する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// true  : 有効
/// false : 無効
/// </returns>
bool FontResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}

