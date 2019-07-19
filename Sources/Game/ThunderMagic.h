#pragma once
#ifndef THUNDER_MAGIC_DEFINED
#define THUNDER_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// 雷魔法クラス
/// </summary>
class ThunderMagic : public Magic {
private:
	static constexpr float THUNDER_MAGIC_RADIUS          = 0.8f;
	static constexpr float THUNDER_MAGIC_COLLIDER_RADIUS = 6.5f;
	static constexpr float THUNDER_MAGIC_CHASE_SPEED     = 0.5f;

public:
	ThunderMagic();
	~ThunderMagic();

public:
	// 雷魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 雷魔法を開放する
	void Lost() override;
	// 雷魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// 雷魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const SphereCollider& collider) override;

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;

};


#endif // !THUNDER_MAGIC_DEFINED

