#pragma once
#ifndef DEATH_EFFECT_DEFINED
#define DEATH_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// ���j�G�t�F�N�g
/// </summary>
class DeathEffect : public Effect {
public:
	// �G�t�F�N�g������������
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;

public:
	// �G�t�F�N�g�̃X�P�[�����擾����
	float GetScale() const;
	// �G�t�F�N�g�̐F�̋������擾����
	float GetColorPower() const;

private:
	// �G�t�F�N�g�̏�����Ԃ̃X�P�[��
	float m_startScale;

};


#endif // !DEATH_EFFECT_DEFINED
