#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include <Utils\Resource.h>
#include "ResourceID.h"


/// <summary>
/// リソース管理クラス
/// </summary>
class ResourceManager final : NonCopyable {
private:
	// 未ロードのリソースのインデックス
	static constexpr int NULL_INDEX = -1;

public:
	// コンストラクタ
	ResourceManager();
	// デストラクタ
	~ResourceManager();

public:
	// リソースをロードする
	void Load();
	// リソースを開放する
	void Release();

	// テクスチャリソースを取得する
	std::shared_ptr<TextureResource> GetTexture(TextureID id);
	// フォントリソースを取得する
	std::shared_ptr<FontResource> GetFont(FontID id);

private:
	// テクスチャリソースを追加する
	bool AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture);
	// フォントリソースを追加する
	bool AddFont(FontID id, std::shared_ptr<FontResource>& font);
	template <class T>
	// リソースを追加する
	void AddResource(std::shared_ptr<T>& resource, int id, std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
		index[id] = resources.size();
		resources.emplace_back(resource);
	}

	template <class T>
	// 有効なリソースか確認する
	bool CheckIsValid(const std::shared_ptr<T>& resource) {
		return resource->IsValid();
	}

	template <class T>
	// リソースをリセットする
	void Reset(std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
		resources.clear();
		resources.shrink_to_fit();
		std::fill(index.begin(), index.end(), NULL_INDEX);
	}

private:
	// テクスチャリソース
	std::vector<std::shared_ptr<TextureResource>>     m_textures;
	// テクスチャリソースの格納位置
	std::vector<int>                                  m_texturesIndex;
	// フォントリソース
	std::vector<std::shared_ptr<FontResource>>        m_fonts;
	// フォントリソースの格納位置
	std::vector<int>                                  m_fontsIndex;

};


#endif // !RESOURCE_MANAGER_DEFINED