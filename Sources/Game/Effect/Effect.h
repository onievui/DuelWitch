#pragma once
#ifndef EFFECT_DEFINED
#define EFFECT_DEFINED


#include "IEffect.h"


/// <summary>
/// エフェクトクラス
/// </summary>
class Effect : public IEffect {
public:
	// コンストラクタ
	Effect() 
		: m_pos()
		, m_vel()
		, m_lifeTime()
		, m_startPos()
		, m_startVel()
		, m_startLifeTime() {
	}

public:
	// エフェクトを初期化する
	virtual void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	virtual void Update(const DX::StepTimer& timer) override;
	// ループする
	virtual void Restart() override;

public:
	// 座標を取得する
	const DirectX::SimpleMath::Vector3& GetPos() const override { return m_pos; }

protected:
	// 座標
	DirectX::SimpleMath::Vector3		m_pos;
	// 速度
	DirectX::SimpleMath::Vector3		m_vel;
	// 加速度
	DirectX::SimpleMath::Vector3        m_accel;
	// エフェクトがループするまでの残り時間
	float								m_lifeTime;

	// 初期座標
	DirectX::SimpleMath::Vector3		m_startPos;
	// 初期速度
	DirectX::SimpleMath::Vector3		m_startVel;
	// 初期加速度
	DirectX::SimpleMath::Vector3        m_startAccel;
	// エフェクトがループするまでの時間
	float								m_startLifeTime;
};


#endif // !EFFECT_DEFINED
