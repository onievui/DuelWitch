#pragma once
#ifndef ELEMENT_PARAMETER_DEFINED
#define ELEMENT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// エレメント用パラメータ
/// </summary>
class ElementParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

private:
	// パラメータを読み込む
	bool LoadData();

public:
	int maxNum;
	float alphaRate;
	float radius;
	float creationInterval;
};


#endif // !ELEMENT_PARAMETER_DEFINED
