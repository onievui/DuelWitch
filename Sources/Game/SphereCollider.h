#pragma once
#ifndef SPHERE_COLLIDER_DEFINED
#define SPHERE_COLLIDER_DEFINED


#include "Collider.h"


class Transform;
class Collision;


/// <summary>
/// �������蔻��N���X
/// </summary>
class SphereCollider : public Collider {
	friend class Collision;
public:
	// �R���X�g���N�^
	SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// �����蔻��̕\��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::White), bool wireframe = true) const;

public:
	// ���a���擾����
	float GetRadius() const;
	// ���a��ݒ肷��
	void SetRadius(float radius);

private:
	// ���a
	float                        m_radius;

private:
	// �����蔻��\���p�I�u�W�F�N�g
	static std::unique_ptr<DirectX::GeometricPrimitive> m_debugSphere;
};


#endif // !SPHERE_COLLIDER_DEFINED







