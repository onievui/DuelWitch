#pragma once
#ifndef COMMAND_PARAMETER_DEFINED
#define COMMAND_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �R�}���h�N���X�p�p�����[�^
/// </summary>
class CommandParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	float moveSpeed;
	float moveSpeedXY;
	float rotSpeed;
	float rotZLimit;
	float rotXLimit;
	float rotYLimit;
	float lerpSpeed;
};

#endif // !COMMAND_PARAMETER_DEFINED
