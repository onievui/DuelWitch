#pragma once
#ifndef NORMAL_MAGIC_EFFECT_DEFINED
#define NORMAL_MAGIC_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �ʏ햂�@�G�t�F�N�g
/// </summary>
class NormalMagicEffect : public Effect {
public:
	// �G�t�F�N�g������������
	void Initialize(float life = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	void Update(const DX::StepTimer& timer) override;

};


#endif // !NORMAL_MAGIC_EFFECT_DEFINED
