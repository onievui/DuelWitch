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
	int maxNum;	                             // �G�������g�̍ő�o����
	float alphaRate;	                     // �G�������g�̕s�����x
	float radius;                            // �G�������g�̔��a
	float creationInterval;                  // �G�������g�̐����Ԋu
	DirectX::SimpleMath::Vector3 appearArea; // �G�������g�̃����_���o���͈�
	float minDistance;                       // �����ɐ�������G�������g�̍ŏ��Ԋu

};


#endif // !ELEMENT_PARAMETER_DEFINED
