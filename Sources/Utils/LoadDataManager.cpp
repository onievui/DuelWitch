#include "LoadDataManager.h"
#include "ErrorMessage.h"
#include "LoadData.h"


/// <summary>
/// コンストラクタ
/// </summary>
LoadDataManager::LoadDataManager()
	: m_loadData() {
}

/// <summary>
/// デストラクタ
/// </summary>
LoadDataManager::~LoadDataManager() {
	m_loadData.clear();
}

/// <summary>
/// データを登録する
/// </summary>
/// <param name="loadData">データ</param>
void LoadDataManager::Regiser(LoadData* loadData) {
	m_loadData.push_back(loadData);
}

/// <summary>
/// データを読み込む
/// </summary>
void LoadDataManager::Load() {
	for (std::vector<LoadData*>::iterator itr = m_loadData.begin(); itr != m_loadData.end(); ++itr) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSONデータの読み込みに失敗しました");
		}
	}
}
