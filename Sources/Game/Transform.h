#pragma once
#ifndef TRANSFORM_DEFINED
#define TRANSFORM_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>


/// <summary>
/// �p���N���X
/// </summary>
class Transform {
public:
	// �R���X�g���N�^
	Transform(const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Quaternion& rotation = DirectX::SimpleMath::Quaternion::Identity,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);
	// �R���X�g���N�^
	Transform(const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& euler,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3& GetLocalPosition() const              { return m_position; }
	virtual const DirectX::SimpleMath::Vector3 GetPosition() const            { return m_position; }
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position)            { m_position = position; }
	// ��]���擾����
	const DirectX::SimpleMath::Quaternion& GetLocalRotation() const           { return m_rotation; }
	virtual const DirectX::SimpleMath::Quaternion GetRotation() const         { return m_rotation; }
	// ��]��ݒ肷��
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation)         { m_rotation = rotation; }
	// �X�P�[�����擾����
	const DirectX::SimpleMath::Vector3& GetLocalScale() const                 { return m_scale; }
	virtual const DirectX::SimpleMath::Vector3 GetScale() const               { return m_scale; }
	// �X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)                  { m_scale = scale; }

	// ���[���h�s����擾����
	virtual DirectX::SimpleMath::Matrix GetMatrix() const;

protected:
	// ���W
	DirectX::SimpleMath::Vector3    m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3    m_scale;

};


/// <summary>
/// �q�p���N���X
/// </summary>
class ChildTransform : public Transform {
public:
	// �R���X�g���N�^
	ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Quaternion& rotation = DirectX::SimpleMath::Quaternion::Identity,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);
	// �R���X�g���N�^
	ChildTransform(const Transform* pParent, const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& euler,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3 GetPosition() const override;
	// ��]���擾����
	const DirectX::SimpleMath::Quaternion GetRotation() const override;
	// �X�P�[�����擾����
	const DirectX::SimpleMath::Vector3 GetScale() const override;
	// ���[���h�s����擾����
	DirectX::SimpleMath::Matrix GetMatrix() const override;
	// �e�I�u�W�F�N�g��ݒ肷��
	void SetParent(const Transform* pParent);

private:
	// �e�I�u�W�F�N�g�ւ̃|�C���^
	const Transform* m_pParent;
};


#endif // !TRANSFORM_DEFINED
