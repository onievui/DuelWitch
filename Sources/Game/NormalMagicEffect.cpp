#include "NormalMagicEffect.h"


/// <summary>
/// エフェクトを初期化する
/// </summary>
/// <param name="lifeTime">ループするまでの時間</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="accel">加速度</param>
void NormalMagicEffect::Initialize(float lifeTime, const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& vel, const DirectX::SimpleMath::Vector3& accel) {
	// 現状何もしない
	lifeTime, pos, vel, accel;
}

/// <summary>
/// エフェクトを更新する
/// </summary>
/// <param name="timer"></param>
void NormalMagicEffect::Update(const DX::StepTimer& timer) {
	// 現状何もしない
	timer;
}
