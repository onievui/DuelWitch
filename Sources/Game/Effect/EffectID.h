#pragma once
#ifndef EFFECT_ID_DEFINED
#define EFFECT_ID_DEFINED


/// <summary>
/// エフェクトのID
/// </summary>
enum class EffectID {
	PlayerTrail,        // プレイヤーの軌道エフェクト
	PlayerCharge,		// プレイヤーのチャージエフェクト
	FieldShield,        // フィールドとの衝突エフェクト
	Hit,				// 魔法ヒットエフェクト
	NormalMagic,        // 通常魔法エフェクト
	FireMagic,	        // 炎魔法エフェクト
	ThunderStrikeMagic, // 落雷魔法エフェクト

	Num,
};


#endif // !EFFECT_ID_DEFINED
