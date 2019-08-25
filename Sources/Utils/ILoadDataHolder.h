#pragma once
#ifndef ILOAD_DATA_HOLDER_DEFINED
#define ILOAD_DATA_HOLDER_DEFINED


#include "LoadDataID.h"


/// <summary>
/// �ǂݍ��݃f�[�^�ێ��C���^�t�F�[�X�N���X
/// </summary>
class ILoadDataHolder {
public:
	// �f�[�^��ǂݍ���
	virtual bool Load() = 0;
	// �f�[�^���ēǂݍ��݂���
	virtual bool Reload() = 0;
	// �f�[�^���J������
	virtual void Dispose() = 0;
	// �ǂݍ��݃f�[�^ID���擾����
	virtual LoadDataID GetID() const = 0;

};


#endif // !ILOAD_DATA_HOLDER_DEFINED
