#pragma once
#ifndef RESOURCE_DEFINED
#define RESOURCE_DEFINED


#include "ResourceID.h"


template <class T, class U>
/// <summary>
/// リソースクラス
/// </summary>
class Resource {
public:
	using Type   = T;
	using IDType = U;

public:
	// コンストラクタ
	Resource() {
		m_resources.clear();
	};
	// デストラクタ
	virtual ~Resource() = default;

public:
	// リソースを取得する
	const T& GetResource(int index = 0) const {
		return static_cast<int>(m_resources.size()) > index ? m_resources[index] : m_defaultResource;
	}

	//リソースを置き換える
	void Replace(T&& resource, int index = 0) {
		m_resources[index] = std::forward<T>(resource);
	}

	// リソースが有効かどうか確認する
	//bool IsValid(int index = 0);

protected:
	// リソース
	std::vector<T> m_resources;
	// リソースが未割り当て時の状態
	static T       m_defaultResource;
};


/// <summary>
/// テクスチャリソース
/// </summary>
class TextureResource : public Resource<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, TextureID> {
public:
	// テクスチャリソースがあるディレクトリ名
	static const std::wstring TEXTURE_DIR;

public:
	// コンストラクタ
	TextureResource(const std::wstring& fileName);
	// デストラクタ
	~TextureResource() = default;

public:
	// リソースを追加する
	void AddResource(const std::wstring& fileName);
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

public:
	// 画像サイズを取得する
	const DirectX::SimpleMath::Vector2& GetSize(int index = 0) const;
	// 画像の中心座標へのオフセットを取得する
	DirectX::SimpleMath::Vector2 GetCenter(int index = 0) const;

private:
	// 画像サイズ
	std::vector<DirectX::SimpleMath::Vector2> m_size;
};


/// <summary>
/// ジオメトリックプリミティブリソース
/// </summary>
class GeometricPrimitiveResource : public Resource<std::unique_ptr<DirectX::GeometricPrimitive>, GeometricPrimitiveID> {
public:
	// コンストラクタ
	GeometricPrimitiveResource(std::unique_ptr<DirectX::GeometricPrimitive>&& resource);
	// ムーブコンストラクタ
	GeometricPrimitiveResource(GeometricPrimitiveResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::GeometricPrimitive>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// デストラクタ
	~GeometricPrimitiveResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

};


/// <summary>
/// モデルリリース
/// </summary>
class ModelResource :public Resource<std::unique_ptr<DirectX::Model>, ModelID> {
public:
	// モデルリソースがあるディレクトリ名
	static const std::wstring MODEL_DIR;

public:
	// コンストラクタ
	ModelResource(const std::wstring& fileName, const std::wstring& directory = L"");
	// ムーブコンストラクタ
	ModelResource(ModelResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::Model>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// デストラクタ
	~ModelResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

};


/// <summary>
/// フォントリソース
/// </summary>
class FontResource : public Resource<std::unique_ptr<DirectX::SpriteFont>, FontID> {
public:
	// フォントリソースがあるディレクトリ名
	static const std::wstring FONT_DIR;

public:
	// コンストラクタ
	FontResource(const std::wstring& fileName);
	// ムーブコンストラクタ
	FontResource(FontResource&& from) {
		for (std::vector<std::unique_ptr<DirectX::SpriteFont>>::iterator itr = from.m_resources.begin();
			itr != from.m_resources.end(); ++itr) {
			m_resources.emplace_back(std::move(*itr));
		}
	}
	// デストラクタ
	~FontResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].get() != m_defaultResource.get(); }

};


/// <summary>
/// 頂点シェーダリソース
/// </summary>
class VertexShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11VertexShader>, VertexShaderID> {
public:
	// 頂点シェーダリソースがあるディレクトリ名
	static const std::wstring VERTEX_SHADER_DIR;

public:
	// コンストラクタ
	VertexShaderResource(const std::wstring& fileName, VertexShaderID id);
	// デストラクタ
	~VertexShaderResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const{ return m_resources[index].Get() != m_defaultResource.Get(); }

public:
	// 入力レイアウトを取得する
	ID3D11InputLayout* GetInputLayout();

private:
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};


/// <summary>
/// ジオメトリシェーダリソース
/// </summary>
class GeometryShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, GeometryShaderID> {
public:
	// 頂点シェーダリソースがあるディレクトリ名
	static const std::wstring GEOMETRY_SHADER_DIR;

public:
	// コンストラクタ
	GeometryShaderResource(const std::wstring& fileName);
	// デストラクタ
	~GeometryShaderResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

};


/// <summary>
/// ピクセルシェーダリソース
/// </summary>
class PixelShaderResource : public Resource<Microsoft::WRL::ComPtr<ID3D11PixelShader>, PixelShaderID> {
public:
	// ピクセルシェーダリソースがあるディレクトリ名
	static const std::wstring PIXEL_SHADER_DIR;

public:
	// コンストラクタ
	PixelShaderResource(const std::wstring& fileName);
	// デストラクタ
	~PixelShaderResource() = default;

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const { return m_resources[index].Get() != m_defaultResource.Get(); }

};


#endif // !RESOURCE_DEFINED
