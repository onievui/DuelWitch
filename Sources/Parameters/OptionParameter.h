#pragma once
#ifndef OPTION_PARAMETER_DEFINED
#define OPTION_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �I�v�V�����p�p�����[�^
/// </summary>
class OptionParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	struct mouse_param {
		float sensivity;
	};
	mouse_param mouseParam;
};


#endif // !OPTION_PARAMETER_DEFINED
