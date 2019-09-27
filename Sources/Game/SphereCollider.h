#pragma once
#ifndef SPHERE_COLLIDER_DEFINED
#define SPHERE_COLLIDER_DEFINED


#include "Collider.h"


class Transform;
class Collision;


/// <summary>
/// 球当たり判定クラス
/// </summary>
class SphereCollider : public Collider {
	friend class Collision;
public:
	// コンストラクタ
	SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// 当たり判定の表示
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::White), bool wireframe = true) const;

public:
	// 半径を取得する
	float GetRadius() const;
	// 半径を設定する
	void SetRadius(float radius);

private:
	// 半径
	float                        m_radius;

private:
	// 当たり判定表示用オブジェクト
	static std::unique_ptr<DirectX::GeometricPrimitive> m_debugSphere;
};


#endif // !SPHERE_COLLIDER_DEFINED







