#pragma once
#ifndef MAGIC_DEFINED
#define MAGIC_DEFINED


#include "IMagic.h"
#include <Game\Object\Transform.h>


class Collider;


/// <summary>
/// ���@�N���X
/// </summary>
class Magic : public IMagic {
public:
	virtual ~Magic() = default;

protected:
	Magic()
		: m_info()
		, m_transform()
		, m_collider()
		, m_world()
		, m_vel()
		, m_color(DirectX::Colors::White)
		, m_isUsed(false) {

	}

public:
	// ���@�𐶐�����
	virtual void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
		m_info = magicInfo;
		m_transform.SetPosition(pos);
		m_vel = dir;
	}
	// ���@���X�V����
	virtual void Update(const DX::StepTimer& timer) { timer; }
	// ���@���J������
	virtual void Lost() {}	
	// ���@��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const { view; proj; }
	// ���@�̏I���������s��
	virtual void Finalize() override {}

public:
	// �I�u�W�F�N�g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const { return m_world; }
	// �I�u�W�F�N�g�̓����蔻����擾����
	const Collider* GetCollider() const                  { return m_collider.get(); }
	// ���@ID���擾����
	MagicID GetID() const                                { return m_info.id; }
	// �v���C���[ID���擾����
	PlayerID GetPlayerID() const                         { return m_info.playerId; }
	// �_���[�W���擾����
	virtual float GetPower() const                       { return m_info.powerRate; }
	// �v���C���[�Ƃ̏Փˏ���
	virtual void HitPlayer(const Collider* collider)     { collider; }
	// ���@�Ƃ̏Փˏ���
	virtual void HitMagic(const IMagic* other)           { other; }
	// ���@���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const                                  { return m_isUsed; }
	// ���@���g�p���邩�ǂ����ݒ肷��
	void SetUsed(bool isUsed)                            { m_isUsed = isUsed; }

protected:
	// ���@�Ɋւ�����
	MagicInfo                                    m_info;
	// �p��
	Transform                                    m_transform;
	// �������蔻��
	std::unique_ptr<Collider>                    m_collider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
	// ���x
	DirectX::SimpleMath::Vector3                 m_vel;
	// �F
	DirectX::SimpleMath::Color                   m_color;
	// �g�p���Ă��邩�ǂ���
	bool                                         m_isUsed;
	// ��������
	float                                        m_lifeTime;

};


#endif // !MAGIC_DEFINED


