#pragma once
#ifndef CAPSULE_COLLIDER_DEFINED
#define CAPSULE_COLLIDER_DEFINED


#include "Collider.h"


class Transform;
class Collision;


/// <summary>
/// �������蔻��N���X
/// </summary>
class CapsuleCollider : public Collider {
	friend class Collision;
public:
	// �R���X�g���N�^
	CapsuleCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& startPos,
		const DirectX::SimpleMath::Vector3& endPos, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// ���a���擾����
	float GetRadius() const      { return m_radius; }
	// ���a��ݒ肷��
	void SetRadius(float radius) { m_radius = radius; }
	// ���ۂ̎n�_���擾����
	DirectX::SimpleMath::Vector3 GetStartPos() const;
	// �n�_���擾����
	const DirectX::SimpleMath::Vector3& GetLocalStartPos() const { return m_pos[0]; }
	// �n�_��ݒ肷��
	void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) { m_pos[0] = startPos; }
	// ���ۂ̏I�_���擾����
	DirectX::SimpleMath::Vector3 GetEndPos() const;
	// �I�_���擾����
	const DirectX::SimpleMath::Vector3& GetLocalEndPos() const { return m_pos[1]; }
	// �I�_��ݒ肷��
	void SetEndPos(const DirectX::SimpleMath::Vector3& endPos) { m_pos[1] = endPos; }


private:
	// ���a
	float                        m_radius;
	// �n�_�ƏI�_
	DirectX::SimpleMath::Vector3 m_pos[2];

};


#endif // !CAPSULE_COLLIDER_DEFINED







