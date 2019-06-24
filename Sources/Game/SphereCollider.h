#pragma once
#ifndef SPHERE_COLLIDER
#define SPHERE_COLLIDER


class Transform;


/// <summary>
/// 球当たり判定クラス
/// </summary>
class SphereCollider {
public:
	// コンストラクタ
	SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// 衝突判定
	bool Collision(const SphereCollider* other) const;
	// 当たり判定の表示
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Vector4& color = DirectX::SimpleMath::Vector4(DirectX::Colors::White)) const;

public:
	// 姿勢クラスへのポインタを取得する
	const Transform* GetTransform() const;
	// 姿勢クラスへのポインタを設定する
	void SetTransform(const Transform* pTransform);
	// 半径を取得する
	float GetRadius() const;
	// 半径を設定する
	void SetRadius(float radius);
	// オフセットを取得する
	const DirectX::SimpleMath::Vector3& GetOffset() const;
	// オフセットを設定する
	void SetOffset(const DirectX::SimpleMath::Vector3& offset);

private:
	// 姿勢クラスへのポインタ
	const Transform*             m_pTransform;
	// 半径
	float                        m_radius;
	// 座標のオフセット
	DirectX::SimpleMath::Vector3 m_offset;

private:
	// 当たり判定表示用オブジェクト
	static std::unique_ptr<DirectX::GeometricPrimitive> m_debugSphere;
};


#endif // !SPHERE_COLLIDER







