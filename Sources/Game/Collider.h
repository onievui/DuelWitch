#pragma once
#ifndef COLLIDER_DEFINED
#define COLLIDER_DEFINED


class Collision;


/// <summary>
/// 当たり判定基底クラス
/// </summary>
class Collider {
	friend class Collision;
public:
	// 当たり判定の種類
	enum class Type : unsigned int {
		Box = 1 << 0,
		Sphere = 1 << 1,
		Capsule = 1 << 2,
		Segment = 1 << 3,
	};

public:
	Collider(Type type) : m_type(type) {}

protected:
	Type m_type;

};


#endif // !COLLIDER_DEFINED
