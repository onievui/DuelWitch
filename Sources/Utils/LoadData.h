#pragma once
#ifndef LOAD_DATA_DEFINED
#define LOAD_DATA_DEFINED


#include "LoadDataID.h"


/// <summary>
/// �ǂݍ��݃f�[�^�N���X
/// </summary>
class LoadData {
public:
	// �R���X�g���N�^
	LoadData(LoadDataID id);
	// �f�X�g���N�^
	virtual ~LoadData() = default;

public:
	// �f�[�^��ǂݍ���
	virtual bool Load() = 0;
	// �f�[�^���J������
	virtual void Dispose() = 0;

public:
	// ID
	const LoadDataID m_id;
	
protected:
	// �ǂݍ��݃f�[�^�{�́i�h���N���X�Œ�`����j
	// std::unique_ptr<_value> m_value;

};


#endif // !LOAD_DATA_DEFINED