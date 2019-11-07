#pragma once
#ifndef COLLIDER_DEFINED
#define COLLIDER_DEFINED


class Collision;
class Transform;


/// <summary>
/// �����蔻����N���X
/// </summary>
class Collider {
	friend class Collision;
public:
	// �����蔻��̎��
	enum class Type : unsigned int {
		Box     = 1 << 0,
		Sphere  = 1 << 1,
		Capsule = 1 << 2,
		Segment = 1 << 3,
	};

protected:
	// �R���X�g���N�^
	Collider(Type type, const Transform* pTransform, const DirectX::SimpleMath::Vector3& offset = DirectX::SimpleMath::Vector3::Zero);

public:
	// �p���ւ̃|�C���^���擾����
	const Transform* GetTransform() const {	return m_pTransform; }
	// �p���ւ̃|�C���^��ݒ肷��
	void SetTransform(const Transform* pTransform) { m_pTransform = pTransform; }
	// �I�t�Z�b�g���擾����
	const DirectX::SimpleMath::Vector3& GetOffset() const { return m_offset; }
	// �I�t�Z�b�g��ݒ肷��
	void SetOffset(const DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }

public:
	// ���ۂ̍��W���擾����
	DirectX::SimpleMath::Vector3 GetPos() const;
	// �����蔻���`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::White), bool wireframe = true) const {
		view, proj, color, wireframe;
	}

protected:
	// �����蔻��̎��
	const Type                   m_type;
	// �p���ւ̃|�C���^
	const Transform*             m_pTransform;
	// ���W�̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_offset;
};


#endif // !COLLIDER_DEFINED
