#pragma once
#ifndef FIRE_MAGIC_DEFINED
#define FIRE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// �����@�N���X
/// </summary>
class FireMagic : public Magic {
private:
	static constexpr float FIRE_MAGIC_RADIUS = 0.8f;

public:
	FireMagic();
	~FireMagic();

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
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const SphereCollider& collider) override;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
};


#endif // !FIRE_MAGIC_DEFINED
