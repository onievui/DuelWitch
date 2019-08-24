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
/// <param name="id">ID</param>
void LoadDataManager::Load(LoadDataID id) {
	// ID�������f�[�^��������������
	auto pred = [id](LoadData* data) {return data->m_id == id; };
	std::vector<LoadData*>::iterator itr = std::find_if(m_loadData.begin(), m_loadData.end(), pred);
	for (; itr != m_loadData.end();) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSON�f�[�^�̓ǂݍ��݂Ɏ��s���܂���");
		}
		itr = std::find_if(itr+1, m_loadData.end(), pred);
	}
}

/// <summary>
/// �f�[�^���J������
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Dispose(LoadDataID id) {
	// ID�������f�[�^��������������
	auto pred = [id](LoadData* data) {return data->m_id == id; };
	std::vector<LoadData*>::iterator itr = std::find_if(m_loadData.begin(), m_loadData.end(), pred);
	for (; itr != m_loadData.end();) {
		(*itr)->Dispose();
		itr = std::find_if(itr+1, m_loadData.end(), pred);
	}
}

