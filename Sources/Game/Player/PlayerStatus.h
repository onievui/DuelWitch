#pragma once
#ifndef PLAYER_STATUS_DEFINED
#define PLAYER_STATUS_DEFINED



/// <summary>
/// プレイヤーステータス
/// </summary>
struct PlayerStatus {
	// 外部データ
	// 最大HP
	float                             maxHp;
	// HP
	float                             hp;
	// 前フレームのHP
	float                             preHp;
	// 最大SP
	float                             maxSp;
	// SP
	float                             sp;
	// 前フレームのSP
	float                             preSp;
	// SP回復速度
	float                             spRecoverySpeed;
	// 通常魔法の消費SP
	float                             normalMagicSpCost;
	// ブースト速度倍率
	float                             boostSpeedRate;
	// ブースト消費SP
	float                             boostSpCost;
	// ロール回避消費SP
	float                             rollSpCost;
	// 1段階目のチャージタイム
	float                             firstChargeTime;
	// 2段階目のチャージタイム
	float                             secoundChargeTime;
	// 炎魔法のダメージ倍率
	float                             fireMagicPowerRate;
	// 氷魔法のダメージ倍率
	float                             freezeMagicPowerRate;
	// 雷魔法のダメージ倍率
	float                             thunderMagicPowerRate;

	// 内部データ
	// キャラクターID
	int                               charaId;
	// ダメージ後無敵時間タイマー
	float                             damageTimer;
	// SP消費直後タイマー
	float                             spDecreaseTimer;
	// ブースト中かどうか
	bool                              isBoosting;
	// チャージ中かどうか
	bool                              isCharging;
	// チャージ段階
	int                               chargeLevel;
	// 詠唱可能かどうか
	bool                              canCast;
};


#endif // !PLAYER_STATUS_DEFINED
