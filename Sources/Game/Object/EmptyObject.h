#pragma once
#ifndef EMPTY_OBJECT_DEFINED
#define EMPTY_OBJECT_DEFINED


#include "IObject.h"
#include "Transform.h"


/// <summary>
/// ��̃I�u�W�F�N�g�N���X
/// </summary>
class EmptyObject : public IObject{
public:
	// �R���X�g���N�^
	EmptyObject();
	// �f�X�g���N�^
	~EmptyObject();

public:
	// �I�u�W�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �I�u�W�F�N�g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override { view, proj; };

public:
	// �I�u�W�F�N�g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override   { return m_world; }
	// �I�u�W�F�N�g�̓����蔻����擾����
	const Collider*                    GetCollider() const override { return m_collider.get(); }

public:
	// �I�u�W�F�N�g�̍s��̎Q�Ƃ��擾����
	DirectX::SimpleMath::Matrix& GetMatrixRef()    { return m_world; }
	// �I�u�W�F�N�g�̎p���̎Q�Ƃ��擾����
	Transform&                   GetTransformRef() { return m_transform; }
	// �I�u�W�F�N�g�̓����蔻��̎Q�Ƃ��擾����
	std::unique_ptr<Collider>&   GetColliderRef()  { return m_collider; }

private:
	// �p��
	Transform                   m_transform;
	// �����蔻��
	std::unique_ptr<Collider>   m_collider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
};


#endif // !EMPTY_OBJECT_DEFINED
