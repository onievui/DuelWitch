#include "LoadData.h"
#include "LoadDataManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="id">ID</param>
LoadData::LoadData(LoadDataID id)
	: m_id(id) {
	LoadDataManager::GetIns()->Regiser(this);
}

