#pragma once
#ifndef HIT_EFFECT_DEFINED
#define HIT_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �q�b�g�G�t�F�N�g
/// </summary>
class HitEffect : public Effect {
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
	// �A���t�@���擾����
	float GetAlpha() const;

};


#endif // !HIT_EFFECT_DEFINED
