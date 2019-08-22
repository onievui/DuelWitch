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
	~TextureResource();

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const;
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
	~GeometricPrimitiveResource();

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0)const;

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
	~ModelResource();

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0)const;

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
	FontResource(const std::wstring& filename);
	// デストラクタ
	~FontResource();

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const;

};


#endif // !RESOURCE_DEFINED
