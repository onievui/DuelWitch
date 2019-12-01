#pragma once
#ifndef THUNDER_STRIKE_EFFECT_EMITTER_DEFINED
#define THUNDER_STRIKE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class ThunderStrikeEffect;
struct BlurParam;


/// <summary>
/// 通常魔法エフェクトエミッタークラス
/// </summary>
class ThuderStrikeEffectEmitter : public EffectEmitter {
public:
	// コンストラクタ
	ThuderStrikeEffectEmitter();

public:
	// エフェクトエミッターを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// エフェクトエミッターを更新する
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// エフェクトエミッターを描画する
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// ガウスの重みを計算する
	float GaussianDistribution(const DirectX::SimpleMath::Vector2& pos, float rho);
	// ブラー用パラメータを計算する
	BlurParam CalcBlurParam(int width, int height, const DirectX::SimpleMath::Vector2& dir, float deviation, float multiply);

private:
	// エフェクト
	std::vector<std::unique_ptr<ThunderStrikeEffect>> m_effects;
	// 視線ベクトル
	DirectX::SimpleMath::Vector3 m_eyeVec;
	// ピクセルシェーダ用定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>      m_cBufferPixel;
	// タイムデータ
	DirectX::SimpleMath::Vector4 m_time;
	// ブラーの方向
	DirectX::SimpleMath::Vector2 m_blurDirection;
};


#endif // !THUNDER_STRIKE_EFFECT_EMITTER_DEFINED
