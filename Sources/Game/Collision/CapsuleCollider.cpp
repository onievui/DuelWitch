#include "CapsuleCollider.h"
#include <Game\Object\Transform.h>


/// <summary>
/// 　コンストラクタ
/// </summary>
/// <param name="pTransform">姿勢へのポインタ</param>
/// <param name="radius">半径</param>
/// <param name="startPos">始点</param>
/// <param name="endPos">終点</param>
/// <param name="offset">座標のオフセット</param>
CapsuleCollider::CapsuleCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& startPos,
	const DirectX::SimpleMath::Vector3& endPos, const DirectX::SimpleMath::Vector3& offset)
	: Collider(Collider::Type::Capsule, pTransform,offset)
	, m_radius(radius) {
	m_pos[0] = startPos;
	m_pos[1] = endPos;
}

/// <summary>
/// 実際の始点を取得する
/// </summary>
/// <returns>
/// 実際の始点
/// </returns>
DirectX::SimpleMath::Vector3 CapsuleCollider::GetStartPos() const {
	Transform tmp = *m_pTransform;
	tmp.SetPosition(tmp.GetLocalPosition() + m_pos[0]);
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return tmp.GetPosition() + offset;
}

/// <summary>
/// 実際の終点を取得する
/// </summary>
/// <returns>
/// 実際の終点
/// </returns>
DirectX::SimpleMath::Vector3 CapsuleCollider::GetEndPos() const {
	Transform tmp = *m_pTransform;
	tmp.SetPosition(tmp.GetLocalPosition() + m_pos[1]);
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return tmp.GetPosition() + offset;
}
