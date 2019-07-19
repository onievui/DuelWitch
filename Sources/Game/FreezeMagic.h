#pragma once
#ifndef FREEZE_MAGIC_DEFINED
#define FREEZE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// �X���@�N���X
/// </summary>
class FreezeMagic : public IMagic {
private:
	static constexpr float FREEZE_MAGIC_RADIUS = 2.5f;

public:
	FreezeMagic();
	~FreezeMagic();

public:
	// �X���@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �X���@���J������
	void Lost() override;
	// �X���@�𐶐�����
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// �X���@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �X���@�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �X���@�̓����蔻����擾����
	const SphereCollider* GetCollider() const override;
	// �v���C���[ID���擾����
	PlayerID GetPlayerID() const override;
	// �X���@���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const;
	// �X���@���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed);

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// �v���C���[ID
	PlayerID                                     m_playerId;
	// �p��
	Transform                                    m_transform;
	// �v���C���[�̍��W
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;
	// ���x
	DirectX::SimpleMath::Vector3                 m_vel;
	// �������蔻��
	SphereCollider                               m_sphereCollider;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix                  m_world;
	// �F
	DirectX::SimpleMath::Vector4                 m_color;
	// �g�p���Ă��邩�ǂ���
	bool                                         m_isUsed;
	// ��������
	float                                        m_lifeTime;
};


#endif // !FREEZE_MAGIC_DEFINED

