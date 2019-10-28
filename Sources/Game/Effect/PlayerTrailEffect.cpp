#include "PlayerTrailEffect.h"
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\Color.h>
#include <Utils\ServiceLocater.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Common\Transform.h>


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void PlayerTrailEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	Effect::Initialize(lifeTime, pos, vel, accel);
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;
	HSVColor color(0, parameter.sColor, parameter.vColor);
	color.setH(RandMt::GetRange(parameter.minHColor, parameter.maxHColor));
	m_color = DirectX::SimpleMath::Color(color.getR()*1.0f, color.getG()*1.0f, color.getB()*1.0f, 255.0f) / 255.0f;
	m_defaultSize = RandMt::GetRange(parameter.minScale, parameter.maxScale);
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void PlayerTrailEffect::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	if (m_delayTime > 0.0f) {
		m_delayTime -= elapsed_time;
		if (m_delayTime > 0.0f) {
			return;
		}
		Restart();
	}

	m_vel += m_accel * elapsed_time;
	m_pos += m_vel * elapsed_time;

	m_lifeTime -= elapsed_time;

	// 時間が無くなったらループする
	if (m_lifeTime < 0) {
		Restart();
	}
}

/// <summary>
/// ループする
/// </summary>
void PlayerTrailEffect::Restart() {
	const EffectParameter::player_trail_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerTrailParam;

	m_lifeTime = m_startLifeTime;

	// 円錐状にランダムな方向を決める
	const float angle = parameter.apexAngle;
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(
		DirectX::SimpleMath::Vector3::Up,
		RandMt::GetRange(-angle * 0.5f, angle*0.5f)
	);

	DirectX::SimpleMath::Vector3 center_dir = DirectX::SimpleMath::Vector3::Transform(
		DirectX::SimpleMath::Vector3::UnitZ, m_pParent->GetRotation());
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(center_dir, rotation);
	direction = DirectX::SimpleMath::Vector3::Transform(direction,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(center_dir, RandMt::GetRand(Math::PI2)));
	direction.Normalize();
	m_pos = m_startPos = m_pParent->GetPosition() + direction * 0.2f;
	m_vel = direction * parameter.speed;
	m_accel = direction * parameter.acceleration;

}

/// <summary>
/// スケールを取得する
/// </summary>
/// <returns>
/// エフェクトのスケール
/// </returns>
float PlayerTrailEffect::GetScale() const {
	return m_defaultSize * (m_lifeTime / m_startLifeTime);
}

/// <summary>
/// 色を取得する
/// </summary>
/// <returns>
/// エフェクトの色
/// </returns>
DirectX::SimpleMath::Color PlayerTrailEffect::GetColor() const {
	return m_color;
}
