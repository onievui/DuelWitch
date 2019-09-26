#pragma once
#ifndef NORMAL_MAGIC_EFFECT_DEFINED
#define NORMAL_MAGIC_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// 通常魔法エフェクト
/// </summary>
class NormalMagicEffect : public Effect {
public:
	// エフェクトを初期化する
	void Initialize(float life = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero) override;
	// エフェクトを更新する
	void Update(const DX::StepTimer& timer) override;

};


#endif // !NORMAL_MAGIC_EFFECT_DEFINED
