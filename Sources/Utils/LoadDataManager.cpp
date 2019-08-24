#include "LoadDataManager.h"
#include "ErrorMessage.h"
#include "ILoadDataHolder.h"


/// <summary>
/// コンストラクタ
/// </summary>
LoadDataManager::LoadDataManager()
	: m_loadDataHolder() {
}

/// <summary>
/// デストラクタ
/// </summary>
LoadDataManager::~LoadDataManager() {
	m_loadDataHolder.clear();
}

/// <summary>
/// データを登録する
/// </summary>
/// <param name="loadDataHolder">データ</param>
void LoadDataManager::Regiser(ILoadDataHolder* loadDataHolder) {
	m_loadDataHolder.push_back(loadDataHolder);
}

/// <summary>
/// データを読み込む
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Load(LoadDataID id) {
	// IDが同じデータだけ処理をする
	auto pred = [id](ILoadDataHolder* data) {return data->GetID() == id; };
	std::vector<ILoadDataHolder*>::iterator itr = std::find_if(m_loadDataHolder.begin(), m_loadDataHolder.end(), pred);
	for (; itr != m_loadDataHolder.end();) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSONデータの読み込みに失敗しました");
		}
		itr = std::find_if(itr+1, m_loadDataHolder.end(), pred);
	}
}

/// <summary>
/// データを開放する
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Dispose(LoadDataID id) {
	// IDが同じデータだけ処理をする
	auto pred = [id](ILoadDataHolder* data) {return data->GetID() == id; };
	std::vector<ILoadDataHolder*>::iterator itr = std::find_if(m_loadDataHolder.begin(), m_loadDataHolder.end(), pred);
	for (; itr != m_loadDataHolder.end();) {
		(*itr)->Dispose();
		itr = std::find_if(itr+1, m_loadDataHolder.end(), pred);
	}
}

