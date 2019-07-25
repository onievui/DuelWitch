#pragma once
#ifndef THUNDER_STRIKE_MAGIC_DEFINED
#define THUNDER_STRIKE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// �������@�N���X
/// </summary>
class ThunderStrikeMagic : public Magic {
private:
	static constexpr float THUNDER_STRIKE_MAGIC_RADIUS = 0.8f;
	static constexpr float THUNDER_STRIKE_MAGIC_LENGTH = 25.0f;

public:
	ThunderStrikeMagic();
	~ThunderStrikeMagic();

public:
	// �������@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// ���������@���J������
	void Lost() override;
	// ���������@�𐶐�����
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) override;
	// ���������@��`�悷��
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


#endif // !THUNDER_STRIKE_STRIKE_MAGIC_DEFINED

