#pragma once
#ifndef DEATH_EFFECT_DEFINED
#define DEATH_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// 撃破エフェクト
/// </summary>
class DeathEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

public:
	// エフェクトのスケールを取得する
	float GetScale() const;
	// エフェクトの色の強さを取得する
	float GetColorPower() const;

private:
	// エフェクトの初期状態のスケール
	float m_startScale;

};


#endif // !DEATH_EFFECT_DEFINED
