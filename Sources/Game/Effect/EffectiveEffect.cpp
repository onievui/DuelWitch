#include "EffectiveEffect.h"
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
void EffectiveEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	const EffectParameter::effective_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->effectiveParam;

	Effect::Initialize(lifeTime, pos, vel, accel);

	m_angle = RandMt::GetRand(Math::PI2);
	m_rotateSpeed = RandMt::GetRange(parameter.minRotateSpeed, parameter.maxRotateSpeed)*Math::PI2*(RandMt::GetRand(2)*2-1);
	m_scale = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void EffectiveEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// 徐々に遅くする
	float t = m_lifeTime / m_startLifeTime;
	t = t * (2 - t);
	m_vel = m_startVel * t;
	m_pos += m_vel * elapsed_time;
	m_angle += m_rotateSpeed*elapsed_time;

	m_lifeTime -= elapsed_time;
}

