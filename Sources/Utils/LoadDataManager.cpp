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
/// <param name="id">ID</param>
void LoadDataManager::Load(LoadDataID id) {
	// IDが同じデータだけ処理をする
	auto pred = [id](LoadData* data) {return data->m_id == id; };
	std::vector<LoadData*>::iterator itr = std::find_if(m_loadData.begin(), m_loadData.end(), pred);
	for (; itr != m_loadData.end();) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSONデータの読み込みに失敗しました");
		}
		itr = std::find_if(itr+1, m_loadData.end(), pred);
	}
}

/// <summary>
/// データを開放する
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Dispose(LoadDataID id) {
	// IDが同じデータだけ処理をする
	auto pred = [id](LoadData* data) {return data->m_id == id; };
	std::vector<LoadData*>::iterator itr = std::find_if(m_loadData.begin(), m_loadData.end(), pred);
	for (; itr != m_loadData.end();) {
		(*itr)->Dispose();
		itr = std::find_if(itr+1, m_loadData.end(), pred);
	}
}

