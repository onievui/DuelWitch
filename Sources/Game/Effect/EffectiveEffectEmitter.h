#pragma once
#ifndef EFFECTIVE_EFFECT_EMITTER_DEFINED
#define EFFECTIVE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class EffectiveEffect;
enum class MagicID;


/// <summary>
/// 打ち消し・反射エフェクトエミッタークラス
/// </summary>
class EffectiveEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	EffectiveEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

public:
	// 魔法IDでエフェクトの色を設定する
	void SetColorID(MagicID id);

private:
	// エフェクト
	std::vector<std::unique_ptr<EffectiveEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3                  m_eyeVec;
	// 色
	DirectX::SimpleMath::Color                    m_color;

};


#endif // !EFFECTIVE_EFFECT_EMITTER_DEFINED
