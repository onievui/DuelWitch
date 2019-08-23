#include "LoadData.h"
#include "LoadDataManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
LoadData::LoadData() {
	LoadDataManager::GetIns()->Regiser(this);
}
