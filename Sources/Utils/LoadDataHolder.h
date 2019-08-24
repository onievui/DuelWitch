#pragma once
#ifndef LOAD_DATA_HOLDER_DEFINED
#define LOAD_DATA_HOLDER_DEFINED


#include "ILoadDataHolder.h"
#include "LoadDataManager.h"


template<class T, LoadDataID DataID>
/// <summary>
/// 読み込みデータ保持クラス
/// </summary>
class LoadDataHolder : public ILoadDataHolder {
	friend class LoadDataManager;

public:
	// 紐づける読み込みデータID
	static constexpr LoadDataID ID = DataID;

public:
	// コンストラクタ
	LoadDataHolder()
		: m_data() {
		LoadDataManager::GetIns()->Regiser(this);
	}

private:
	// データを読み込む
	bool Load() override {
		if (!m_data) {
			m_data = std::make_unique<T>();
		}
		return m_data->Load();
	}

	// データを開放する
	void Dispose() override {
		m_data.reset();
	}

	// 読み込みデータIDを取得する
	LoadDataID GetID() const override {
		return ID;
	}

public:
	T* operator->() {
		return m_data.get();
	}

private:
	// 読み込むデータ本体
	std::unique_ptr<T> m_data;

};


#endif // !LOAD_DATA_HOLDER_DEFINED
