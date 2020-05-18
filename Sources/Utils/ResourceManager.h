#pragma once
#ifndef RESOURCE_MANAGER_DEFINED
#define RESOURCE_MANAGER_DEFINED


#include <Utils\NonCopyable.h>
#include "Resource.h"
#include "ErrorMessage.h"


template <class T>
/// <summary>
/// リソースマネージャクラス
/// </summary>
class ResourceManager final : NonCopyable {
public:
	using ResourceType = typename T::Type;
	using IDType       = typename T::IDType;

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

	template<class... Args>
	bool AddResource(IDType id, Args&&... args) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"の取得に失敗しました";
			ErrorMessage((m_kind + error_message).c_str());
			return false;
		}
		m_resources[index]->AddResource(args...);
		return true;
	}

	// リソースを解放する
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

	// 非constのリソースを取得する
	T* GetRawResource(IDType id) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"の取得に失敗しました";
			ErrorMessage((m_kind + error_message).c_str());
			return nullptr;
		}
		return m_resources[index].get();
	}

	// 全てのリソースを取得する
	const std::vector<std::unique_ptr<T>>& GetAllResources() { return m_resources; }

	// 非constの全てのリソースを取得する
	std::vector<std::unique_ptr<T>>& GetRawAllResources() { return m_resources; }

	// リソースを置き換える
	void Replace(IDType id, ResourceType&& resource, int index2 = 0) {
		int index = m_index[static_cast<int>(id)];
		if (index == NULL_INDEX) {
			std::wstring error_message = L"の取得に失敗しました";
			ErrorMessage((m_kind + error_message).c_str());
			return;
		}
		T* data = m_resources[index].get();
		data->Replace(std::forward<ResourceType>(resource), index2);
	}

private:
	// リソース
	std::vector<std::unique_ptr<T>> m_resources;
	// リソースの格納位置
	std::vector<int>                m_index;
	// リソースの種類
	const std::wstring              m_kind;

};


#endif // !RESOURCE_MANAGER_DEFINED