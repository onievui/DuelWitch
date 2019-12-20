#pragma once
#ifndef HIT_EFFECT_DEFINED
#define HIT_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// ヒットエフェクト
/// </summary>
class HitEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

public:
	// スケールを取得する
	float GetScale() const;
	// アルファを取得する
	float GetAlpha() const;

};


#endif // !HIT_EFFECT_DEFINED
