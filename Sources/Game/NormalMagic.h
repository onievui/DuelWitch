#pragma once
#ifndef NORMAL_MAGIC_DEFINED
#define NORMAL_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "IMagic.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// �ʏ햂�@�N���X
/// </summary>
class NormalMagic : public IMagic {
private:
	static constexpr float NORMAL_MAGIC_RADIUS = 0.8f;

public:
	NormalMagic();
	~NormalMagic();

public:
	// �ʏ햂�@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �ʏ햂�@���J������
	void Lost() override;
	// �ʏ햂�@�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// �ʏ햂�@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �ʏ햂�@�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// �ʏ햂�@�̓����蔻����擾����
	const SphereCollider* GetCollider() const override;
	// �ʏ햂�@���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const;
	// �ʏ햂�@���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed);

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
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


#endif // !NORMAL_MAGIC_DEFINED

