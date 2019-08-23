#include "LoadDataManager.h"
#include "ErrorMessage.h"
#include "LoadData.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadDataManager::LoadDataManager()
	: m_loadData() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LoadDataManager::~LoadDataManager() {
	m_loadData.clear();
}

/// <summary>
/// �f�[�^��o�^����
/// </summary>
/// <param name="loadData">�f�[�^</param>
void LoadDataManager::Regiser(LoadData* loadData) {
	m_loadData.push_back(loadData);
}

/// <summary>
/// �f�[�^��ǂݍ���
/// </summary>
void LoadDataManager::Load() {
	for (std::vector<LoadData*>::iterator itr = m_loadData.begin(); itr != m_loadData.end(); ++itr) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSON�f�[�^�̓ǂݍ��݂Ɏ��s���܂���");
		}
	}
}
