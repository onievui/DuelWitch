#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED


#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


enum class ElementID;


/// <summary>
/// �G�������g�N���X
/// </summary>
class Element : public IObject {
public:
	static constexpr float ELEMENT_RADIUS = 0.75f;

public:
	Element();
	~Element();

public:
	// �G�������g���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �G�������g���J������
	void Lost() override;
	// �G�������g�𐶐�����
	void Create(ElementID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color);
	// �G�������g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �G�������g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �G�������g�̓����蔻����擾����
	const SphereCollider* GetCollider() const override;
	// �G�������g��ID���擾����
	ElementID GetID() const;
	// �G�������g���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const;
	// �G�������g���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed);

private:
	// �G�������gID
	ElementID                                    m_id;
	// �p��
	Transform                                    m_transform;
	// �������蔻��
	SphereCollider                               m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
	// �F
	DirectX::SimpleMath::Vector4                 m_color;
	// �g�p���Ă��邩�ǂ���
	bool                                         m_isUsed;
};


#endif // !ELEMENT_DEFINED


