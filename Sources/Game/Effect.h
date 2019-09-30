#pragma once
#ifndef EFFECT_DEFINED
#define EFFECT_DEFINED


#include "IEffect.h"


/// <summary>
/// �G�t�F�N�g�N���X
/// </summary>
class Effect : public IEffect {
public:
	// �R���X�g���N�^
	Effect() 
		: m_pos()
		, m_vel()
		, m_lifeTime()
		, m_startPos()
		, m_startVel()
		, m_startLifeTime() {
	}

public:
	// �G�t�F�N�g������������
	virtual void Initialize(float lifeTime = 0, const DirectX::SimpleMath::Vector3& pos = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& vel = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& accel = DirectX::SimpleMath::Vector3::Zero) override;
	// �G�t�F�N�g���X�V����
	virtual void Update(const DX::StepTimer& timer) override;
	// ���[�v����
	virtual void Restart() override;

public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3& GetPos() const override { return m_pos; }

protected:
	// ���W
	DirectX::SimpleMath::Vector3		m_pos;
	// ���x
	DirectX::SimpleMath::Vector3		m_vel;
	// �����x
	DirectX::SimpleMath::Vector3        m_accel;
	// �G�t�F�N�g�����[�v����܂ł̎c�莞��
	float								m_lifeTime;

	// �������W
	DirectX::SimpleMath::Vector3		m_startPos;
	// �������x
	DirectX::SimpleMath::Vector3		m_startVel;
	// ���������x
	DirectX::SimpleMath::Vector3        m_startAccel;
	// �G�t�F�N�g�����[�v����܂ł̎���
	float								m_startLifeTime;
};


#endif // !EFFECT_DEFINED
