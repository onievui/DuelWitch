#pragma once
#ifndef LOAD_DATA_MANAGER_DEFINED
#define LOAD_DATA_MANAGER_DEFINED


#include "Singleton.h"
#include "LoadDataID.h"


class ILoadDataHolder;


/// <summary>
/// �ǂݍ��݃f�[�^�}�l�[�W���N���X
/// </summary>
class LoadDataManager : public Singleton<LoadDataManager> {
	friend class Singleton<LoadDataManager>;

public:
	// �R���X�g���N�^
	LoadDataManager();
	// �f�X�g���N�^
	~LoadDataManager();

public:
	// �f�[�^�̓o�^
	void Regiser(ILoadDataHolder* loadDataHolder);
	// �f�[�^��ǂݍ���
	void Load(LoadDataID id);
	// �f�[�^���J������
	void Dispose(LoadDataID id);

private:
	// �ǂݍ��ݑΏۂ̃f�[�^
	std::vector<ILoadDataHolder*> m_loadDataHolder;

};


#endif // !LOAD_DATA_MANAGER_DEFINED
