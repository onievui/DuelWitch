#pragma once
#ifndef SPHERE_COLLIDER
#define SPHERE_COLLIDER


class Transform;


/// <summary>
/// �������蔻��N���X
/// </summary>
class SphereCollider {
public:
	// �R���X�g���N�^
	SphereCollider(const Transform* pTransform, float radius, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// �Փ˔���
	bool Collision(const SphereCollider* other) const;
	// �����蔻��̕\��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Vector4& color = DirectX::SimpleMath::Vector4(DirectX::Colors::White)) const;

public:
	// �p���N���X�ւ̃|�C���^���擾����
	const Transform* GetTransform() const;
	// �p���N���X�ւ̃|�C���^��ݒ肷��
	void SetTransform(const Transform* pTransform);
	// ���a���擾����
	float GetRadius() const;
	// ���a��ݒ肷��
	void SetRadius(float radius);
	// �I�t�Z�b�g���擾����
	const DirectX::SimpleMath::Vector3& GetOffset() const;
	// �I�t�Z�b�g��ݒ肷��
	void SetOffset(const DirectX::SimpleMath::Vector3& offset);

private:
	// �p���N���X�ւ̃|�C���^
	const Transform*             m_pTransform;
	// ���a
	float                        m_radius;
	// ���W�̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_offset;

private:
	// �����蔻��\���p�I�u�W�F�N�g
	static std::unique_ptr<DirectX::GeometricPrimitive> m_debugSphere;
};


#endif // !SPHERE_COLLIDER







