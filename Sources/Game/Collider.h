#pragma once
#ifndef COLLIDER_DEFINED
#define COLLIDER_DEFINED


class Collision;


/// <summary>
/// �����蔻����N���X
/// </summary>
class Collider {
	friend class Collision;
public:
	// �����蔻��̎��
	enum class Type : unsigned int {
		Box = 1 << 0,
		Sphere = 1 << 1,
		Capsule = 1 << 2,
		Segment = 1 << 3,
	};

protected:
	Collider(Type type) : m_type(type) {}

protected:
	Type m_type;

};


#endif // !COLLIDER_DEFINED
