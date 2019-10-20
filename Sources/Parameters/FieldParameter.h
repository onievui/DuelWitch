#pragma once
#ifndef FIELD_PARAMETER_DEFINED
#define FIELD_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// フィールド用パラメータ
/// </summary>
class FieldParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	float startScaleDownTime;
	float scaleDownSpeed;
	float defaultScale;
	float minScale;
};


#endif // !FIELD_PARAMETER_DEFINED
