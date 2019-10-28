#pragma once
#ifndef PLAYER_TRAIL_EFFECT_DEFINED
#define PLAYER_TRAIL_EFFECT_DEFINED


#include "Effect.h"


class Transform;


/// <summary>
/// �v���C���[�̋O���G�t�F�N�g
/// </summary>
class PlayerTrailEffect : public Effect {
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
	// �e�I�u�W�F�N�g��ݒ肷��
	void SetParent(const Transform* pParent) { m_pParent = pParent; }
	// �X�P�[�����擾����
	float GetScale() const;
	// �F���擾����
	DirectX::SimpleMath::Color GetColor() const;
	// �J�n�x�����Ԃ�ݒ肷��
	void SetDelayTime(float delayTime) { m_delayTime = delayTime; }
	// �J�n�҂����ǂ����擾����
	bool IsWaiting() const { return m_delayTime > 0.0f; }

private:
	// �e�I�u�W�F�N�g
	const Transform*           m_pParent;
	// �F
	DirectX::SimpleMath::Color m_color;
	// �����T�C�Y
	float                      m_defaultSize;
	// �J�n�x������
	float                      m_delayTime;
};


#endif // !PLAYER_TRAIL_EFFECT_DEFINED
