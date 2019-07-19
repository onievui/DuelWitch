#pragma once
#ifndef FREEZE_MAGIC_DEFINED
#define FREEZE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// 氷魔法クラス
/// </summary>
class FreezeMagic : public IMagic {
private:
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
	// 氷魔法の行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// 氷魔法の当たり判定を取得する
	const SphereCollider* GetCollider() const override;
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const override;
	// 氷魔法を使用しているかどうか取得する
	bool IsUsed() const;
	// 氷魔法を使用するかどうか設定する
	void IsUsed(bool isUsed);

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// プレイヤーID
	PlayerID                                     m_playerId;
	// 姿勢
	Transform                                    m_transform;
	// プレイヤーの座標
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;
	// 速度
	DirectX::SimpleMath::Vector3                 m_vel;
	// 球当たり判定
	SphereCollider                               m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
	// 色
	DirectX::SimpleMath::Vector4                 m_color;
	// 使用しているかどうか
	bool                                         m_isUsed;
	// 生存時間
	float                                        m_lifeTime;
};


#endif // !FREEZE_MAGIC_DEFINED

