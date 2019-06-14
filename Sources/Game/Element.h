#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"

/// <summary>
/// �G�������g�N���X
/// </summary>
class Element : public IObject{
public:
	Element();
	~Element();

public:
	// �G�������g���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �G�������g���J������
	void Lost() override;
	// �G�������g�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos);
	// �G�������g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �G�������g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const;

private:
	static constexpr float ELEMENT_RADIUS = 1.5f;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// �p��
	Transform                                    m_transform;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
};


#endif // !ELEMENT_DEFINED


