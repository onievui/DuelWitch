#pragma once
#ifndef MAGIC_DEFINED
#define MAGIC_DEFINED


#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// 魔法クラス
/// </summary>
class Magic : public IMagic {
public:
	virtual ~Magic() = default;

protected:
	Magic(MagicID id) 
		: m_id(id)
		, m_playerId()
		, m_transform()
		, m_sphereCollider(&m_transform, 0.0f)
		, m_world()
		, m_vel()
		, m_color()
		, m_isUsed(false) {

	}

public:
	// 魔法を更新する
	virtual void Update(const DX::StepTimer& timer) { timer; }
	// 魔法を開放する
	virtual void Lost() {}
	// 魔法を生成する
	virtual void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) {
		m_playerId = playerId;
		m_transform.SetPosition(pos);
		m_vel = dir;
		m_color = color;
	}
	// 魔法を描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const { view; proj; }

public:
	// オブジェクトの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const { return m_world; }
	// オブジェクトの当たり判定を取得する
	const Collider* GetCollider() const { return &m_sphereCollider; }
	// 魔法IDを取得する
	MagicID GetID() const { return m_id; }
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const { return m_playerId; }
	// プレイヤーとの衝突処理
	virtual void HitPlayer(const Collider* collider) { collider; }
	// 魔法との衝突処理
	virtual void HitMagic(const IMagic* other) { other; }
	// 魔法を使用しているかどうか取得する
	bool IsUsed() const { return m_isUsed; }
	// 魔法を使用するかどうか設定する
	void IsUsed(bool isUsed) { m_isUsed = isUsed; }

protected:
	// 魔法ID
	MagicID                                      m_id;
	// プレイヤーID
	PlayerID                                     m_playerId;
	// 姿勢
	Transform                                    m_transform;
	// 球当たり判定
	SphereCollider                               m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
	// 速度
	DirectX::SimpleMath::Vector3                 m_vel;
	// 色
	DirectX::SimpleMath::Vector4                 m_color;
	// 使用しているかどうか
	bool                                         m_isUsed;
	// 生存時間
	float                                        m_lifeTime;

};


#endif // !MAGIC_DEFINED


