#include "DeathEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void DeathEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	Effect::Initialize(lifeTime, pos, vel, accel);

	m_startScale = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer"></param>
void DeathEffect::Update(const DX::StepTimer& timer) {
	Effect::Update(timer);
}

/// <summary>
/// エフェクトのスケールを取得する
/// </summary>
/// <returns>
/// スケール
/// </returns>
float DeathEffect::GetScale() const {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	float t = std::sinf(Math::PI2*(m_startLifeTime - m_lifeTime)*parameter.blinkSpeed);
	return (t*0.5f + 1.0f)*m_startScale;
}

/// <summary>
/// エフェクトの色の強さを取得する
/// </summary>
/// <returns>
/// 色の強さ
/// </returns>
float DeathEffect::GetColorPower() const {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;

	float t = std::sinf(Math::PI2*(m_startLifeTime - m_lifeTime)*parameter.blinkSpeed);
	return (t + 1.0f)*0.5f*0.7f + 0.3f;
}
