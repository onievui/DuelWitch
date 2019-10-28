#pragma once
#ifndef FREEZE_MAGIC_DEFINED
#define FREEZE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// 氷魔法クラス
/// </summary>
class FreezeMagic : public Magic {
public:
	// コンストラクタ
	FreezeMagic();
	// デストラクタ
	~FreezeMagic();

public:
	// 氷魔法を生成する
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// 氷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 氷魔法を開放する
	void Lost() override;
	// 氷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// ダメージを取得する
	float GetPower() const override;
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// プレイヤーの座標
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;

};


#endif // !FREEZE_MAGIC_DEFINED

