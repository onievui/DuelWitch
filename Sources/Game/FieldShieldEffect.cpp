#include "FieldShieldEffect.h"
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">消えるまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void FieldShieldEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	vel, accel;

	m_startLifeTime = m_lifeTime = lifeTime;
	m_pos = pos;
	m_isUsed = true;
}

void FieldShieldEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_lifeTime -= elapsed_time;

	// 時間が無くなったら未使用にする
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// エフェクトのスケール
/// </returns>
float FieldShieldEffect::GetScale() const {
	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;
	return parameter.scale + (m_startLifeTime - m_lifeTime)*parameter.scaleSpeed;
}

/// <summary>
/// 色を取得する
/// </summary>
/// <returns>
/// エフェクトの色
/// </returns>
DirectX::SimpleMath::Color FieldShieldEffect::GetColor() const {
	DirectX::SimpleMath::Color color = DirectX::Colors::Aqua;
	float t = m_lifeTime / m_startLifeTime;
	float t2 = (t < 0.5f ? t2 = (0.5f - t)*2.0f : (t - 0.5f)*2.0f);

	color.w = 1.0f - t2*t2*t2;

	return color;
}
