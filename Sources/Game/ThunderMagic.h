#pragma once
#ifndef THUNDER_MAGIC_DEFINED
#define THUNDER_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// �����@�N���X
/// </summary>
class ThunderMagic : public IMagic {
private:
	static constexpr float THUNDER_MAGIC_RADIUS = 0.8f;

public:
	ThunderMagic();
	~ThunderMagic();

public:
	// �����@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �����@���J������
	void Lost() override;
	// �����@�𐶐�����
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// �����@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �����@�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �����@�̓����蔻����擾����
	const SphereCollider* GetCollider() const override;
	// �v���C���[ID���擾����
	PlayerID GetPlayerID() const override;
	// �����@���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const;
	// �����@���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed);

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// �v���C���[ID
	PlayerID                                     m_playerId;
	// �p��
	Transform                                    m_transform;
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


#endif // !THUNDER_MAGIC_DEFINED

