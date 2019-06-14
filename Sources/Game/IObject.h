#pragma once
#ifndef IOBJECT_DEFINED
#define IOBJECT_DEFINED

#include <Framework\StepTimer.h>


/// <summary>
/// �I�u�W�F�N�g�N���X
/// </summary>
class IObject {
public:
	virtual ~IObject() = default;

public:
	// �I�u�W�F�N�g���X�V����
	virtual void Update(const DX::StepTimer& timer) = 0;
	// �I�u�W�F�N�g���J������
	virtual void Lost() = 0;
	// �I�u�W�F�N�g��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const = 0;

public:
	// �I�u�W�F�N�g�̍s����擾����
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() const = 0;
};


#endif // !IOBJECT_DEFINED


