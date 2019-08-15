#pragma once
#ifndef RESOURCE_DEFINED
#define RESOURCE_DEFINED


template <class T>
/// <summary>
/// リソースクラス
/// </summary>
class Resource {
public:
	using Type = T;
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
class TextureResource : public Resource<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> {
public:
	// テクスチャリソースがあるディレクトリ名
	static const std::wstring TEXTURE_DIR;

public:
	// コンストラクタ
	TextureResource(const std::wstring& filename);
	// デストラクタ
	~TextureResource();

public:
	// リソースが有効かどうか確認する
	bool IsValid(int index = 0) const;
};

/// <summary>
/// フォントリソース
/// </summary>
class FontResource : public Resource<std::unique_ptr<DirectX::SpriteFont>> {
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
