#pragma once
#ifndef OPTION_PARAMETER_DEFINED
#define OPTION_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// オプション用パラメータ
/// </summary>
class OptionParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	struct mouse_param {
		float sensivity;
	};
	mouse_param mouseParam;
};


#endif // !OPTION_PARAMETER_DEFINED
