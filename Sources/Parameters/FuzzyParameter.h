#pragma once
#ifndef FUZZY_PARAMETER_DEFINED
#define FUZZY_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// �t�@�W�[�p�p�����[�^
/// </summary>
class FuzzyParameter : public ILoadData {
public:
	// �p�����[�^��ǂݍ���
	bool Load() override;
	// �p�����[�^���ēǂݍ��݂���
	bool Reload() override;
	// �p�����[�^��ۑ�����
	bool Save() override;

public:
	struct ai_move_param {
		float hpGapMin;
		float hpGapMax;
		float distanceMin;
		float distanceMax;
		float hasElementNumMin;
		float hasElementNumMax;
		float elementDistanceMin;
		float elementDistanceMax;
	};
	ai_move_param aiMoveParam;

};

#endif // !FUZZY_PARAMETER_DEFINED
