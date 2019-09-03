//#pragma once
//#ifndef EMPTY_OBJECT_DEFINED
//#define EMPTY_OBJECT_DEFINED
//
//
//#include "IObject.h"
//
//
//class Transform;
//
//
///// <summary>
///// ��̃I�u�W�F�N�g�N���X
///// </summary>
//class EmptyObject : public IObject{
//public:
//	// �R���X�g���N�^
//	EmptyObject() = default;
//	// �f�X�g���N�^
//	~EmptyObject() = default;
//
//public:
//	// �I�u�W�F�N�g���X�V����
//	void Update(const DX::StepTimer& timer) override { timer; }
//	// �I�u�W�F�N�g���J������
//	void Lost() override {};
//	// �I�u�W�F�N�g��`�悷��
//	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override { view, proj; };
//
//public:
//	// �I�u�W�F�N�g�̍s����擾����
//	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
//	// �I�u�W�F�N�g�̓����蔻����擾����
//	const SphereCollider* GetCollider() const override;
//
//private:
//	Transform m_trans
//	SphereCollider m_collider;
//};
//
//
//#endif // !EMPTY_OBJECT_DEFINED
