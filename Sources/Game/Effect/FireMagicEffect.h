#pragma once
#ifndef FIRE_MAGIC_EFFECT_DEFINED
#define FIRE_MAGIC_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �����@�G�t�F�N�g
/// </summary>
class FireMagicEffect : public Effect {
public:
	// �G�t�F�N�g������������
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;
	// ���[�v����
	void Restart() override;

public:
	// ��]���擾����
	float GetAngle() const { return m_angle; }
	// �X�P�[�����擾����
	float GetScale() const;
	// �F���擾����
	DirectX::SimpleMath::Color GetColor() const;

private:
	// ��]
	float m_angle;
};


#endif // !FIRE_MAGIC_EFFECT_DEFINED
