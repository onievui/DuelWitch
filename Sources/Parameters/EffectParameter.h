#pragma once
#ifndef EFFECT_PARAMETER_DEFINED
#define EFFECT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// エフェクトクラス用パラメータ
/// </summary>
class EffectParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	std::vector<int> maxNum;

	struct normal_param {

	};
	normal_param normalParam;

	struct fire_param {

	};
	fire_param fireParam;

	struct freeze_param {

	};
	freeze_param freezeParam;

	struct thunder_param {

	};
	thunder_param thunderParam;

	struct thunder_strike_param {

	};
	thunder_strike_param thunderStrikeParam;
};

#endif // !EFFECT_PARAMETER_DEFINED
