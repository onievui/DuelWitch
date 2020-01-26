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
	// キャラクターのパラメータ
	struct chara_param {
		float maxHp;                    // 最大HP
		float maxSp;                    // 最大SP
		float spRecoverySpeed;          // SP回復速度
		float normalMagicSpCost;	    // 通常魔法の消費SP
		float boostSpeedRate;	        // ブースト移動時の速度倍率
		float boostSpCost;              // ブースト移動の消費SP(秒)
		float rollSpCost;               // ロール回避の消費SP
		float quickTurnSpCost;          // クイックターンの消費SP
		float firstChargeTime;          // 1段階チャージまでの時間
		float secoundChargeTime;        // 2段階チャージまでの時間
		float fireMagicPowerRate;       // 炎魔法の攻撃倍率
		float freezeMagicPowerRate;     // 氷魔法の攻撃倍率
		float thunderMagicPowerRate;	// 雷魔法の攻撃倍率
	};
	std::vector<chara_param> charaParams;

};

#endif // !CHARA_STATUS_PARAMETER_DEFINED
