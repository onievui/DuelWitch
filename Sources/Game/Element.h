#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED

#include <Framework\StepTimer.h>

#include "Transform.h"

/// <summary>
/// �G�������g�N���X
/// </summary>
class Element {
public:
	Element();
	~Element();

	// �G�������g���X�V����
	void Update(const DX::StepTimer& timer);
	// �G�������g���J������
	void Lost();
	// �G�������g�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos);
	// �G�������g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

private:
	static constexpr float s_radius = 1.5f;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// �p��
	Transform                                    m_transform;
};


#endif // !ELEMENT_DEFINED


