#pragma once
#ifndef MAGIC_DEFINED
#define MAGIC_DEFINED


#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// ���@�N���X
/// </summary>
class Magic : public IMagic {
public:
	virtual ~Magic() = default;

protected:
	Magic() 
		: m_playerId()
		, m_transform()
		, m_sphereCollider(&m_transform, 0.0f)
		, m_world()
		, m_vel()
		, m_color()
		, m_isUsed(false) {

	}

public:
	// ���@���X�V����
	virtual void Update(const DX::StepTimer& timer) { timer; }
	// ���@���J������
	virtual void Lost() {}
	// ���@�𐶐�����
	virtual void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) {
		m_playerId = playerId;
		m_transform.SetPosition(pos);
		m_vel = vel;
		m_color = color;
	}
	// ���@��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const { view; proj; }

public:
	// �I�u�W�F�N�g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const { return m_world; }
	// �I�u�W�F�N�g�̓����蔻����擾����
	const SphereCollider* GetCollider() const { return &m_sphereCollider; }
	// �v���C���[ID���擾����
	PlayerID GetPlayerID() const { return m_playerId; }
	// �v���C���[�Ƃ̏Փˏ���
	virtual void HitPlayer(const SphereCollider& collider) { collider; }
	// ���@���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const { return m_isUsed; }
	// ���@���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed) { m_isUsed = isUsed; }

protected:
	// �v���C���[ID
	PlayerID                                     m_playerId;
	// �p��
	Transform                                    m_transform;
	// �������蔻��
	SphereCollider                               m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
	// ���x
	DirectX::SimpleMath::Vector3                 m_vel;
	// �F
	DirectX::SimpleMath::Vector4                 m_color;
	// �g�p���Ă��邩�ǂ���
	bool                                         m_isUsed;
	// ��������
	float                                        m_lifeTime;

};


#endif // !MAGIC_DEFINED


