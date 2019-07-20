#pragma once
#ifndef THUNDER_MAGIC_DEFINED
#define THUNDER_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class MagicManager;


/// <summary>
/// �����@�N���X
/// </summary>
class ThunderMagic : public Magic {
private:
	static constexpr float THUNDER_MAGIC_RADIUS          = 0.8f;
	static constexpr float THUNDER_MAGIC_COLLIDER_RADIUS = 6.5f;
	static constexpr float THUNDER_MAGIC_CHASE_SPEED     = 0.5f;

public:
	ThunderMagic(MagicManager* magicManager);
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
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const SphereCollider& collider) override;

private:
	// �I�u�W�F�N�g
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager*                                m_pMagicManager;
	// ���������p�^�C�}�[�쓮�t���O
	bool                                         m_startTimer;
	// ���������p�^�C�}�[
	float                                        m_time;
};


#endif // !THUNDER_MAGIC_DEFINED

