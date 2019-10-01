#pragma once
#ifndef ELEMENT_PARAMETER_DEFINED
#define ELEMENT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �G�������g�p�p�����[�^
/// </summary>
class ElementParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

private:
	// �p�����[�^��ǂݍ���
	bool LoadData();

public:
	int maxNum;
	float alphaRate;
	float radius;
	float creationInterval;
};


#endif // !ELEMENT_PARAMETER_DEFINED
