#pragma once
#ifndef ILOAD_DATA_DEFINED
#define ILOAD_DATA_DEFINED


/// <summary>
/// �ǂݍ��݃f�[�^�C���^�t�F�[�X�N���X
/// </summary>
class ILoadData {
public:
	// �f�[�^��ǂݍ���
	virtual bool Load() = 0;
	// �f�[�^���ēǂݍ��݂���
	virtual bool Reload() = 0;
	// �f�[�^��ۑ�����
	virtual bool Save() = 0;
};

#endif // !ILOAD_DATA_DEFINED
