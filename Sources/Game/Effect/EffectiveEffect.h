#pragma once
#ifndef EFFECTIVE_EFFECT_DEFINED
#define EFFECTIVE_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �ł������E���˃G�t�F�N�g
/// </summary>
class EffectiveEffect : public Effect {
public:
	// �G�t�F�N�g������������
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;

public:
	// ��]���擾����
	float GetAngle() const { return m_angle; }
	// �X�P�[�����擾����
	float GetScale() const { return m_scale; }

private:
	// ��]
	float m_angle;
	// ��]���x
	float m_rotateSpeed;
	// �X�P�[��
	float m_scale;

};


#endif // !EFFECTIVE_EFFECT_DEFINED
