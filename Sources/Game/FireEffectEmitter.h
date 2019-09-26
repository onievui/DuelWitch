#pragma once
#ifndef FIRE_MAGIC_EFFECT_EMITTER_DEFINED
#define FIRE_MAGIC_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


/// <summary>
/// 炎魔法エフェクトエミッタークラス
/// </summary>
class FireMagicEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	FireMagicEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:

};


#endif // !FIRE_MAGIC_EFFECT_EMITTER_DEFINED
