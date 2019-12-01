#pragma once
#ifndef THUNDER_STRIKE_EFFECT_DEFINED
#define THUNDER_STRIKE_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// 通常魔法エフェクト
/// </summary>
class ThunderStrikeEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

};


#endif // !THUNDER_STRIKE_EFFECT_DEFINED
