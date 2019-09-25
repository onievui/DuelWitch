#include "Resource.h"
#include <Framework\DirectX11.h>
#include "ServiceLocater.h"
#include "ErrorMessage.h"
#include "BinFile.h"
#include "InputLayoutData.h"


const std::wstring TextureResource::TEXTURE_DIR                = L"Resources/Textures/";
const std::wstring ModelResource::MODEL_DIR                    = L"Resources/Models/";
const std::wstring FontResource::FONT_DIR                      = L"Resources/Fonts/";
const std::wstring VertexShaderResource::VERTEX_SHADER_DIR     = L"Resources/Shader/";
const std::wstring GeometryShaderResource::GEOMETRY_SHADER_DIR = L"Resources/Shader/";
const std::wstring PixelShaderResource::PIXEL_SHADER_DIR       = L"Resources/Shader/";

TextureResource::Type Resource<TextureResource::Type, TextureResource::IDType>::m_defaultResource = nullptr;
GeometricPrimitiveResource::Type Resource<GeometricPrimitiveResource::Type, GeometricPrimitiveResource::IDType>::m_defaultResource = nullptr;
ModelResource::Type Resource<ModelResource::Type, ModelResource::IDType>::m_defaultResource = nullptr;
FontResource::Type Resource<FontResource::Type, FontResource::IDType>::m_defaultResource = nullptr;
VertexShaderResource::Type Resource<VertexShaderResource::Type, VertexShaderResource::IDType>::m_defaultResource = nullptr;
GeometryShaderResource::Type Resource<GeometryShaderResource::Type, GeometryShaderResource::IDType>::m_defaultResource = nullptr;
PixelShaderResource::Type Resource<PixelShaderResource::Type, PixelShaderResource::IDType>::m_defaultResource = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">読み込む画像のファイル名</param>
TextureResource::TextureResource(const std::wstring& fileName) {
	AddResource(fileName);
}

/// <summary>
/// リソースを追加する
/// </summary>
/// <param name="fileName">ファイル名</param>
void TextureResource::AddResource(const std::wstring& fileName) {
	m_resources.emplace_back(nullptr);
	// 画像のロード
	DirectX::CreateWICTextureFromFile(ServiceLocater<DirectX11>::Get()->GetDevice().Get(),
		(TEXTURE_DIR + fileName).c_str(), nullptr, m_resources.back().GetAddressOf());
	if (m_resources.back().Get() == m_defaultResource.Get()) {
		ErrorMessage(L"画像の読み込みに失敗しました");
	}

	// 画像サイズを調べる
	ID3D11ShaderResourceView* texture = m_resources.back().Get();
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	texture->GetResource(resource.GetAddressOf());
	D3D11_RESOURCE_DIMENSION dimension;
	resource->GetType(&dimension);
	// Texture2Dでない場合は処理しない
	if (dimension != D3D11_RESOURCE_DIMENSION_TEXTURE2D) {
		m_size.emplace_back(DirectX::SimpleMath::Vector2());
		return;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	resource.As(&texture2D);
	D3D11_TEXTURE2D_DESC desc;
	texture2D->GetDesc(&desc);
	m_size.emplace_back(DirectX::SimpleMath::Vector2(static_cast<float>(desc.Width), static_cast<float>(desc.Height)));
}

/// <summary>
/// 画像サイズを取得する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// 画像サイズ
/// </returns>
const DirectX::SimpleMath::Vector2& TextureResource::GetSize(int index) const {
	if (static_cast<int>(m_resources.size()) < index) {
		ErrorMessage(L"画像サイズの取得で範囲外にアクセスしました");
	}
	return m_size[index];
}

/// <summary>
/// 画像の中心座標へのオフセットを取得する
/// </summary>
/// <param name="index">リソースの番号</param>
/// <returns>
/// 中心座標へのオフセット
/// </returns>
DirectX::SimpleMath::Vector2 TextureResource::GetCenter(int index) const {
	if (static_cast<int>(m_resources.size()) < index) {
		ErrorMessage(L"画像の中心座標へのオフセットの取得で範囲外にアクセスしました");
		return DirectX::SimpleMath::Vector2(0, 0);
	}
	return m_size[index] * 0.5f;
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
/// コンストラクタ
/// </summary>
/// <param name="fileName">ファイル名</param>
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
/// コンストラクタ
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <param name="id">頂点シェーダID</param>
VertexShaderResource::VertexShaderResource(const std::wstring& fileName, VertexShaderID id) {
	// 頂点シェーダの読み込み
	BinFile vs_data = BinFile((VERTEX_SHADER_DIR + fileName).c_str());

	// 頂点シェーダを作成する
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	m_resources.emplace_back(nullptr);
	if (FAILED(device->CreateVertexShader(vs_data.Data(), vs_data.Size(), NULL, m_resources.back().GetAddressOf()))) {
		ErrorMessage(L"頂点シェーダの作成に失敗しました");
		return;
	}

	// 入力レイアウトを作成する
	std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc = InputElementDesc::Create(id);
	device->CreateInputLayout(&input_element_desc[0],
		input_element_desc.size(),
		vs_data.Data(), vs_data.Size(),
		m_inputLayout.GetAddressOf());
	
}

/// <summary>
/// 入力レイアウトを取得する
/// </summary>
/// <returns>
/// 入力レイアウト
/// </returns>
ID3D11InputLayout* VertexShaderResource::GetInputLayout() {
	return m_inputLayout.Get();
}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">ファイル名</param>
GeometryShaderResource::GeometryShaderResource(const std::wstring& fileName) {
	// ジオメトリシェーダの読み込み
	BinFile gs_data = BinFile((GEOMETRY_SHADER_DIR + fileName).c_str());

	// ジオメトリシェーダを作成する
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	m_resources.emplace_back(nullptr);
	if (FAILED(device->CreateGeometryShader(gs_data.Data(), gs_data.Size(), NULL, m_resources.back().GetAddressOf()))) {
		ErrorMessage(L"ジオメトリシェーダの作成に失敗しました");
	}

}


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="fileName">ファイル名</param>
PixelShaderResource::PixelShaderResource(const std::wstring& fileName) {
	// ピクセルシェーダの読み込み
	BinFile ps_data = BinFile((PIXEL_SHADER_DIR + fileName).c_str());

	// ピクセルシェーダを作成する
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	m_resources.emplace_back(nullptr);
	if (FAILED(device->CreatePixelShader(ps_data.Data(), ps_data.Size(), NULL, m_resources.back().GetAddressOf()))) {
		ErrorMessage(L"ピクセルシェーダの作成に失敗しました");
	}
}
