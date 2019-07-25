#pragma once
#ifndef FREEZE_MAGIC_DEFINED
#define FREEZE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// �X���@�N���X
/// </summary>
class FreezeMagic : public Magic {
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
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const SphereCollider& collider) override;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// �v���C���[�̍��W
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;

};


#endif // !FREEZE_MAGIC_DEFINED
