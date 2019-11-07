#pragma once
#ifndef COLLIDER_DEFINED
#define COLLIDER_DEFINED


class Collision;
class Transform;


/// <summary>
/// 当たり判定基底クラス
/// </summary>
class Collider {
	friend class Collision;
public:
	// 当たり判定の種類
	enum class Type : unsigned int {
		Box     = 1 << 0,
		Sphere  = 1 << 1,
		Capsule = 1 << 2,
		Segment = 1 << 3,
	};

protected:
	// コンストラクタ
	Collider(Type type, const Transform* pTransform, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// 姿勢へのポインタを取得する
	const Transform* GetTransform() const {	return m_pTransform; }
	// 姿勢へのポインタを設定する
	void SetTransform(const Transform* pTransform) { m_pTransform = pTransform; }
	// オフセットを取得する
	const DirectX::SimpleMath::Vector3& GetOffset() const { return m_offset; }
	// オフセットを設定する
	void SetOffset(const DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }

public:
	// 実際の座標を取得する
	DirectX::SimpleMath::Vector3 GetPos() const;
	// 当たり判定を描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::White), bool wireframe = true) const {
		view, proj, color, wireframe;
	}

protected:
	// 当たり判定の種類
	const Type                   m_type;
	// 姿勢へのポインタ
	const Transform*             m_pTransform;
	// 座標のオフセット
	DirectX::SimpleMath::Vector3 m_offset;
};


#endif // !COLLIDER_DEFINED
