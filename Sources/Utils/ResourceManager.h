#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include "Resource.h"


template <class T>
class ResourceManager final : NonCopyable {
public:
	using IDType = typename T::IDType;

private:
	// 未ロードのリソースのインデックス
	static constexpr int NULL_INDEX = -1;

public:
	// コンストラクタ
	ResourceManager(const std::wstring& kind) 
		: m_kind(kind) {
		Release();
		m_index.resize(static_cast<int>(IDType::Num), NULL_INDEX);
	}


	// デストラクタ
	~ResourceManager() {

	};

public:
	// リソースを追加する
	bool AddResource(IDType id, T&& resource) {
		if (!resource.IsValid()) {
			std::wstring error_message = L"の読み込みに失敗しました";
			ErrorMessage((m_kind + error_message).c_str());
			return false;
		}
		m_index[static_cast<int>(id)] = m_resources.size();
		m_resources.emplace_back(std::make_unique<T>(std::forward<T>(resource)));
		return true;
	}

	// リソースを開放する
	void Release() {
		m_resources.clear();
		m_resources.shrink_to_fit();
		std::fill(m_index.begin(), m_index.end(), NULL_INDEX);
	}

	// リソースを取得する
	const T* GetResource(IDType id) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"の取得に失敗しました";
			ErrorMessage((m_kind + error_message).c_str());
			return nullptr;
		}
		return m_resources[index].get();
	}

private:
	// リソース
	std::vector<std::unique_ptr<T>> m_resources;
	// リソースの格納位置
	std::vector<int>                m_index;
	// リソースの種類
	std::wstring                    m_kind;

};


///// <summary>
///// リソース管理クラス
///// </summary>
//class ResourceManager final : NonCopyable {
//private:
//	// 未ロードのリソースのインデックス
//	static constexpr int NULL_INDEX = -1;
//
//public:
//	// コンストラクタ
//	ResourceManager();
//	// デストラクタ
//	~ResourceManager();
//
//public:
//	// リソースをロードする
//	void Load();
//	// リソースを開放する
//	void Release();
//
//	// テクスチャリソースを取得する
//	std::shared_ptr<TextureResource> GetTexture(TextureID id);
//	// フォントリソースを取得する
//	std::shared_ptr<FontResource> GetFont(FontID id);
//
//private:
//	// テクスチャリソースを追加する
//	bool AddTexture(TextureID id, std::shared_ptr<TextureResource>& texture);
//	// フォントリソースを追加する
//	bool AddFont(FontID id, std::shared_ptr<FontResource>& font);
//	template <class T>
//	// リソースを追加する
//	void AddResource(std::shared_ptr<T>& resource, int id, std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
//		index[id] = resources.size();
//		resources.emplace_back(resource);
//	}
//
//	template <class T>
//	// 有効なリソースか確認する
//	bool CheckIsValid(const std::shared_ptr<T>& resource) {
//		return resource->IsValid();
//	}
//
//	template <class T>
//	// リソースをリセットする
//	void Reset(std::vector<std::shared_ptr<T>>& resources, std::vector<int>& index) {
//		resources.clear();
//		resources.shrink_to_fit();
//		std::fill(index.begin(), index.end(), NULL_INDEX);
//	}
//
//private:
//	// テクスチャリソース
//	std::vector<std::shared_ptr<TextureResource>>     m_textures;
//	// テクスチャリソースの格納位置
//	std::vector<int>                                  m_texturesIndex;
//	// フォントリソース
//	std::vector<std::shared_ptr<FontResource>>        m_fonts;
//	// フォントリソースの格納位置
//	std::vector<int>                                  m_fontsIndex;
//
//};


#endif // !RESOURCE_MANAGER_DEFINED