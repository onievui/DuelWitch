#include "LoadData.h"
#include "LoadDataManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadData::LoadData() {
	LoadDataManager::GetIns()->Regiser(this);
}
