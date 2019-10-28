#include "Collider.h"
#include <Game\Common\Transform.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="type">当たり判定の種類</param>
/// <param name="pTransform">姿勢クラスへのポインタ</param>
/// <param name="offset">座標のオフセット</param>
Collider::Collider(Type type, const Transform* pTransform, const DirectX::SimpleMath::Vector3& offset) 
	: m_type(type)
	, m_pTransform(pTransform)
	, m_offset(offset) {
}

/// <summary>
/// 実際の座標を取得する
/// </summary>
/// <returns>
/// 実際の座標を取得する
/// </returns>
DirectX::SimpleMath::Vector3 Collider::GetPos() const {
	DirectX::SimpleMath::Vector3 offset = DirectX::SimpleMath::Vector3::Transform(m_offset, m_pTransform->GetRotation());
	return m_pTransform->GetPosition() + offset;
}
