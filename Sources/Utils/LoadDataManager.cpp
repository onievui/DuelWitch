#include "LoadDataManager.h"
#include "ErrorMessage.h"
#include "ILoadDataHolder.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
LoadDataManager::LoadDataManager()
	: m_loadDataHolder() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LoadDataManager::~LoadDataManager() {
	m_loadDataHolder.clear();
}

/// <summary>
/// �f�[�^��o�^����
/// </summary>
/// <param name="loadDataHolder">�f�[�^</param>
void LoadDataManager::Register(ILoadDataHolder* loadDataHolder) {
	m_loadDataHolder.push_back(loadDataHolder);
}

/// <summary>
/// �f�[�^����������
/// </summary>
/// <param name="loadDataHolder">�f�[�^</param>
void LoadDataManager::Unregister(ILoadDataHolder* loadDataHolder) {
	std::vector<ILoadDataHolder*>::iterator result = std::remove_if(m_loadDataHolder.begin(), m_loadDataHolder.end(),
		[loadDataHolder](ILoadDataHolder* data) {return data == loadDataHolder; });
	m_loadDataHolder.erase(result, m_loadDataHolder.end());
}

/// <summary>
/// �f�[�^��ǂݍ���
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Load(LoadDataID id) {
	// ID�������f�[�^��������������
	auto pred = [id](ILoadDataHolder* data) {return data->GetID() == id; };
	std::vector<ILoadDataHolder*>::iterator itr = std::find_if(m_loadDataHolder.begin(), m_loadDataHolder.end(), pred);
	for (; itr != m_loadDataHolder.end();) {
		if (!(*itr)->Load()) {
			ErrorMessage(L"JSON�f�[�^�̓ǂݍ��݂Ɏ��s���܂���");
		}
		itr = std::find_if(itr+1, m_loadDataHolder.end(), pred);
	}
}

/// <summary>
/// �f�[�^���ēǂݍ��݂���
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Reload(LoadDataID id) {
	// ID�������f�[�^��������������
	auto pred = [id](ILoadDataHolder* data) {return data->GetID() == id; };
	std::vector<ILoadDataHolder*>::iterator itr = std::find_if(m_loadDataHolder.begin(), m_loadDataHolder.end(), pred);
	for (; itr != m_loadDataHolder.end();) {
		if (!(*itr)->Reload()) {
			ErrorMessage(L"JSON�f�[�^�̍ēǂݍ��݂Ɏ��s���܂���");
		}
		itr = std::find_if(itr + 1, m_loadDataHolder.end(), pred);
	}
}

/// <summary>
/// �f�[�^���������
/// </summary>
/// <param name="id">ID</param>
void LoadDataManager::Dispose(LoadDataID id) {
	// ID�������f�[�^��������������
	auto pred = [id](ILoadDataHolder* data) {return data->GetID() == id; };
	std::vector<ILoadDataHolder*>::iterator itr = std::find_if(m_loadDataHolder.begin(), m_loadDataHolder.end(), pred);
	for (; itr != m_loadDataHolder.end();) {
		(*itr)->Dispose();
		itr = std::find_if(itr+1, m_loadDataHolder.end(), pred);
	}
}

