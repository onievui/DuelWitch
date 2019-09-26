#pragma once
#ifndef NORMAL_MAGIC_EFFECT_EMITTER_DEFINED
#define NORMAL_MAGIC_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


/// <summary>
/// 通常魔法エフェクトエミッタークラス
/// </summary>
class NormalMagicEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	NormalMagicEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// 視線ベクトル
	DirectX::SimpleMath::Vector3 m_eyeVec;
};


#endif // !NORMAL_MAGIC_EFFECT_EMITTER_DEFINED
