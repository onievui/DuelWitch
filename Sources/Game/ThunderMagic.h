#pragma once
#ifndef THUNDER_MAGIC_DEFINED
#define THUNDER_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// 雷魔法クラス
/// </summary>
class ThunderMagic : public IMagic {
private:
	static constexpr float THUNDER_MAGIC_RADIUS = 0.8f;

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
	// 雷魔法の行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// 雷魔法の当たり判定を取得する
	const SphereCollider* GetCollider() const override;
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const override;
	// 雷魔法を使用しているかどうか取得する
	bool IsUsed() const;
	// 雷魔法を使用するかどうか設定する
	void IsUsed(bool isUsed);

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// プレイヤーID
	PlayerID                                     m_playerId;
	// 姿勢
	Transform                                    m_transform;
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


#endif // !THUNDER_MAGIC_DEFINED

