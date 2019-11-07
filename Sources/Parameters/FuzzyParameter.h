#pragma once
#ifndef FUZZY_PARAMETER_DEFINED
#define FUZZY_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// ファジー用パラメータ
/// </summary>
class FuzzyParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
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
