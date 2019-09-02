#include "Resource.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "ErrorMessage.h"


const std::wstring TextureResource::TEXTURE_DIR = L"Resources/Textures/";
const std::wstring ModelResource::MODEL_DIR     = L"Resources/Models/";
const std::wstring FontResource::FONT_DIR       = L"Resources/Fonts/";

TextureResource::Type Resource<TextureResource::Type, TextureResource::IDType>::m_defaultResource = nullptr;
GeometricPrimitiveResource::Type Resource<GeometricPrimitiveResource::Type, GeometricPrimitiveResource::IDType>::m_defaultResource = nullptr;
ModelResource::Type Resource<ModelResource::Type, ModelResource::IDType>::m_defaultResource = nullptr;
FontResource::Type Resource<FontResource::Type, FontResource::IDType>::m_defaultResource = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">読み込む画像のファイル名</param>
TextureResource::TextureResource(const std::wstring& fileName) {
	AddResource(fileName);
}

/// <summary>
/// デストラクタ
/// </summary>
TextureResource::~TextureResource() {
}

/// <summary>
/// リソースを追加する
/// </summary>
/// <param name="fileName">読み込む画像のファイル名</param>
void TextureResource::AddResource(const std::wstring& fileName) {
	m_resources.emplace_back(nullptr);
	DirectX::CreateWICTextureFromFile(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(TEXTURE_DIR + fileName).c_str(), nullptr, m_resources.back().GetAddressOf());
	if (m_resources.back().Get() == m_defaultResource.Get()) {
		ErrorMessage(L"画像の読み込みに失敗しました");
	}
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
/// <param name="resource">ジオメトリックプリミティブ</param>
GeometricPrimitiveResource::GeometricPrimitiveResource(std::unique_ptr<DirectX::GeometricPrimitive>&& resource) {
	m_resources.emplace_back(std::move(resource));
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"ジオメトリックプリミティブの生成に失敗しました");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
GeometricPrimitiveResource::~GeometricPrimitiveResource() {
}

/// <summary>
/// リソースが有効かどうか確認する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// true  : 有効
/// false : 無効
/// </returns>
bool GeometricPrimitiveResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <param name="directory">ディレクトリ</param>
ModelResource::ModelResource(const std::wstring& fileName, const std::wstring& directory) {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	// エフェクトファクトリーを作成する
	std::unique_ptr<DirectX::EffectFactory> fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// 読み込むのファイルのディレクトリを設定する
	fxFactory.get()->SetDirectory((MODEL_DIR + directory).c_str());
	// CMOを読み込んでモデルを作成する
	m_resources.emplace_back(DirectX::Model::CreateFromCMO(device, (MODEL_DIR + directory + L"/" + fileName).c_str(), *fxFactory));
	if (m_resources.back().get() == m_defaultResource.get()) {
		ErrorMessage(L"モデルのに読み込み失敗しました");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
ModelResource::~ModelResource() {

}

/// <summary>
/// リソースが有効かどうか確認する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// true  : 有効
/// false : 無効
/// </returns>
bool ModelResource::IsValid(int index) const {
	return m_resources[index].get() != m_defaultResource.get();
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">読み込むフォントファイル</param>
FontResource::FontResource(const std::wstring& fileName) {
	m_resources.emplace_back(
		std::make_unique<DirectX::SpriteFont>(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(FONT_DIR + fileName).c_str())
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

