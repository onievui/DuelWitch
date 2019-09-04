#pragma once
#ifndef FIRE_MAGIC_DEFINED
#define FIRE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// 炎魔法クラス
/// </summary>
class FireMagic : public Magic {
public:
	FireMagic();
	~FireMagic();

public:
	// 炎魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 炎魔法を開放する
	void Lost() override;
	// 炎魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// 炎魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

};


#endif // !FIRE_MAGIC_DEFINED

