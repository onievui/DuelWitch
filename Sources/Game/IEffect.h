#pragma once
#ifndef IEFFECT_DEFINED
#define IEFFECT_DEFINED


#include <Framework\StepTimer.h>


/// <summary>
/// エフェクトインタフェースクラス
/// </summary>
class IEffect {
public:
	// エフェクトを初期化する
	virtual void Initialize(float life = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero) = 0;
	// エフェクトを更新する
	virtual void Update(const DX::StepTimer& timer) = 0;

public:
	// 座標を取得する
	virtual const DirectX::SimpleMath::Vector3& GetPos() const = 0;

};


#endif // !IEFFECT_DEFINED
