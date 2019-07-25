#pragma once
#ifndef NORMAL_MAGIC_DEFINED
#define NORMAL_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// �ʏ햂�@�N���X
/// </summary>
class NormalMagic : public Magic {
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
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// �ʏ햂�@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const SphereCollider& collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
};


#endif // !NORMAL_MAGIC_DEFINED

