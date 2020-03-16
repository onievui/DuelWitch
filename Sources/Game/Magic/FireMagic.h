#pragma once
#ifndef FIRE_MAGIC_DEFINED
#define FIRE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class IEffectEmitter;


/// <summary>
/// 炎魔法クラス
/// </summary>
class FireMagic : public Magic {
public:
	// ロックオン時の追尾をしない時間
	static constexpr float LOCKON_DISABLE_TIME = 0.25f;

public:
	// コンストラクタ
	FireMagic();
	// デストラクタ
	~FireMagic();

public:
	// 炎魔法を生成する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// 炎魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 炎魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// 炎魔法の終了処理を行う
	void Finalize() override;

public:
	// ダメージを取得する
	float GetPower() const override;
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// 炎魔法エフェクト
	IEffectEmitter* m_pEffect;
	// ロックオン用タイマー
	float           m_lockOnTimer;

};


#endif // !FIRE_MAGIC_DEFINED

