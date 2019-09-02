#pragma once
#ifndef THUNDER_STRIKE_MAGIC_DEFINED
#define THUNDER_STRIKE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// 落雷魔法クラス
/// </summary>
class ThunderStrikeMagic : public Magic {
public:
	ThunderStrikeMagic();
	~ThunderStrikeMagic();

public:
	// 落雷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 落雷雷魔法を開放する
	void Lost() override;
	// 落雷雷魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// 落雷雷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const SphereCollider& collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

};


#endif // !THUNDER_STRIKE_STRIKE_MAGIC_DEFINED

