#pragma once
#ifndef LOAD_DATA_DEFINED
#define LOAD_DATA_DEFINED


/// <summary>
/// �ǂݍ��݃f�[�^
/// </summary>
class LoadData {
public:
	// �R���X�g���N�^
	LoadData();
	// �f�X�g���N�^
	virtual ~LoadData() = default;

public:
	// �f�[�^��ǂݍ���
	virtual bool Load() = 0;

};


#endif // !LOAD_DATA_DEFINED