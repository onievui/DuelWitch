#pragma once
#ifndef FIELD_PARAMETER_DEFINED
#define FIELD_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �t�B�[���h�p�p�����[�^
/// </summary>
class FieldParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	float startScaleDownTime;
	float scaleDownSpeed;
	float defaultScale;
	float minScale;
};


#endif // !FIELD_PARAMETER_DEFINED
