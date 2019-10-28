#pragma once
#ifndef CAPSULE_COLLIDER_DEFINED
#define CAPSULE_COLLIDER_DEFINED


#include "Collider.h"


class Transform;
class Collision;


/// <summary>
/// 球当たり判定クラス
/// </summary>
class CapsuleCollider : public Collider {
	friend class Collision;
public:
	// コンストラクタ
	CapsuleCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& startPos,
		const DirectX::SimpleMath::Vector3& endPos, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// 半径を取得する
	float GetRadius() const      { return m_radius; }
	// 半径を設定する
	void SetRadius(float radius) { m_radius = radius; }
	// 実際の始点を取得する
	DirectX::SimpleMath::Vector3 GetStartPos() const;
	// 始点を取得する
	const DirectX::SimpleMath::Vector3& GetLocalStartPos() const { return m_pos[0]; }
	// 始点を設定する
	void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) { m_pos[0] = startPos; }
	// 実際の終点を取得する
	DirectX::SimpleMath::Vector3 GetEndPos() const;
	// 終点を取得する
	const DirectX::SimpleMath::Vector3& GetLocalEndPos() const { return m_pos[1]; }
	// 終点を設定する
	void SetEndPos(const DirectX::SimpleMath::Vector3& endPos) { m_pos[1] = endPos; }


private:
	// 半径
	float                        m_radius;
	// 始点と終点
	DirectX::SimpleMath::Vector3 m_pos[2];

};


#endif // !CAPSULE_COLLIDER_DEFINED







