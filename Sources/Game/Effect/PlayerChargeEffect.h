#pragma once
#ifndef PLAYER_CHARGE_EFFECT_DEFINED
#define PLAYER_CHARGE_EFFECT_DEFINED


#include "Effect.h"


/// <summary>
/// �v���C���[�̃`���[�W�G�t�F�N�g
/// </summary>
class PlayerChargeEffect : public Effect {
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
	// �X�P�[�����擾����
	float GetScale() const;
	// �J�n�x�����Ԃ�ݒ肷��
	void SetDelayTime(float delayTime) { m_delayTime = delayTime; }
	// �J�n�҂����ǂ����擾����
	bool IsWaiting() const { return m_delayTime > 0.0f; }

private:
	// �J�n�x������
	float                      m_delayTime;

};


#endif // !PLAYER_CHARGE_EFFECT_DEFINED
