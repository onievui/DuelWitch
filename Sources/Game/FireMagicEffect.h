#pragma once
#ifndef FIRE_MAGIC_EFFECT_DEFINED
#define FIRE_MAGIC_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// 炎魔法エフェクト
/// </summary>
class FireMagicEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;
	// ループする
	void Restart() override;

public:
	// 回転を取得する
	float GetAngle() const { return m_angle; }
	// スケールを取得する
	float GetScale() const;
	// 色を取得する
	DirectX::SimpleMath::Color GetColor() const;

private:
	// 回転
	float m_angle;
};


#endif // !FIRE_MAGIC_EFFECT_DEFINED
