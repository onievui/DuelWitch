#pragma once
#ifndef NORMAL_MAGIC_DEFINED
#define NORMAL_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class IEffectEmitter;


/// <summary>
/// 通常魔法クラス
/// </summary>
class NormalMagic : public Magic {
public:
	// コンストラクタ
	NormalMagic();
	// デストラクタ
	~NormalMagic();

public:
	// 通常魔法を生成する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// 通常魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 通常魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// 通常魔法の終了処理を行う
	void Finalize() override;

public:
	// ダメージを取得する
	float GetPower() const override;
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// 通常魔法のエフェクト
	IEffectEmitter* m_pEffect;

};


#endif // !NORMAL_MAGIC_DEFINED

