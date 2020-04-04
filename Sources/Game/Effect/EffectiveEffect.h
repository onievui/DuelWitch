#pragma once
#ifndef EFFECTIVE_EFFECT_DEFINED
#define EFFECTIVE_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// 打ち消し・反射エフェクト
/// </summary>
class EffectiveEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

public:
	// 回転を取得する
	float GetAngle() const { return m_angle; }
	// スケールを取得する
	float GetScale() const { return m_scale; }

private:
	// 回転
	float m_angle;
	// 回転速度
	float m_rotateSpeed;
	// スケール
	float m_scale;

};


#endif // !EFFECTIVE_EFFECT_DEFINED
