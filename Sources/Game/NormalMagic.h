#pragma once
#ifndef NORMAL_MAGIC_DEFINED
#define NORMAL_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// 通常魔法クラス
/// </summary>
class NormalMagic : public Magic {
public:
	NormalMagic();
	~NormalMagic();

public:
	// 通常魔法を更新する
	void Update(const DX::StepTimer& timer) override;
	// 通常魔法を開放する
	void Lost() override;
	// 通常魔法を生成する
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// 通常魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーとの衝突処理
	void HitPlayer(const Collider* collider) override;
	// 魔法との衝突処理
	void HitMagic(const IMagic* other) override;

};


#endif // !NORMAL_MAGIC_DEFINED

