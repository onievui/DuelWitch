#pragma once
#ifndef IEFFECT_DEFINED
#define IEFFECT_DEFINED


#include <Framework\StepTimer.h>


/// <summary>
/// �G�t�F�N�g�C���^�t�F�[�X�N���X
/// </summary>
class IEffect {
public:
	// �G�t�F�N�g������������
	virtual void Initialize(float life = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero) = 0;
	// �G�t�F�N�g���X�V����
	virtual void Update(const DX::StepTimer& timer) = 0;

public:
	// ���W���擾����
	virtual const DirectX::SimpleMath::Vector3& GetPos() const = 0;

};


#endif // !IEFFECT_DEFINED
