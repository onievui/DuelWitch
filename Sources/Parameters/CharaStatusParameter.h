#pragma once
#ifndef CHARA_STATUS_PARAMETER_DEFINED
#define CHARA_STATUS_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// キャラステータス用パラメータ
/// </summary>
class CharaStatusParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

public:
	struct chara1_param {
		float maxHp;
		float maxSp;
		float spRecoverySpeed;
		float normalMagicSpCost;
		float boostSpeedRate;
		float boostSpCost;
	};
	chara1_param chara1Param;

};

#endif // !CHARA_STATUS_PARAMETER_DEFINED
