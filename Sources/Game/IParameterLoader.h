#pragma once
#ifndef IPARAMETER_LOADER_DEFINED
#define IPARAMETER_LOADER_DEFINED


/// <summary>
/// �p�����[�^�ǂݍ��݃C���^�t�F�[�X�N���X
/// </summary>
class IParameterLoader {
public:
	// �p�����[�^��ǂݍ���
	virtual void Load() = 0;
	// �p�����[�^���ēǂݍ��݂���
	virtual void Reload() = 0;
	// �p�����[�^���J������
	virtual void Dispose() = 0;
};


#endif // !IPARAMETER_LOADER_DEFINED
