#pragma once
#ifndef FIELD_SHIELD_EFFECT_DEFINED
#define FIELD_SHIELD_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �t�B�[���h�Փ˃G�t�F�N�g
/// </summary>
class FieldShieldEffect : public Effect {
public:
	// �G�t�F�N�g������������
	void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;

public:
	// �X�P�[�����擾����
	float GetScale() const;
	// �F���擾����
	DirectX::SimpleMath::Color GetColor() const;
	// �g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const { return m_isUsed; }

private:
	// �g�p���Ă��邩�ǂ����̃t���O
	bool m_isUsed;

};


#endif // !FIELD_SHIELD_EFFECT_DEFINED
