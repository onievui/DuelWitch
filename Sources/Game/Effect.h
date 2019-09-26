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
	void Initialize(float life = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero) override { life, pos, vel; }
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override { timer; }

public:
	// 座標を取得する
	const DirectX::SimpleMath::Vector3& GetPos() const override { return m_pos; }

protected:
	// 座標
	DirectX::SimpleMath::Vector3		m_pos;
	// 速度
	DirectX::SimpleMath::Vector3		m_vel;
	// エフェクトがループするまでの残り時間
	float								m_lifeTime;

	// 初期座標
	DirectX::SimpleMath::Vector3		m_startPos;
	// 初期速度
	DirectX::SimpleMath::Vector3		m_startVel;
	// エフェクトがループするまでの時間
	float								m_startLifeTime;
};


#endif // !EFFECT_DEFINED
