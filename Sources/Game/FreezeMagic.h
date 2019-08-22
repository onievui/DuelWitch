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
	static constexpr float FREEZE_MAGIC_RADIUS = 2.5f;

public:
	FreezeMagic();
	~FreezeMagic();

public:
	// 氷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 氷魔法を開放する
	void Lost() override;
	// 氷魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// 氷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const SphereCollider& collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

private:
	// プレイヤーの座標
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;

};


#endif // !FREEZE_MAGIC_DEFINED

