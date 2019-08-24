#include "LoadData.h"
#include "LoadDataManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="id">ID</param>
LoadData::LoadData(LoadDataID id)
	: m_id(id) {
	LoadDataManager::GetIns()->Regiser(this);
}

