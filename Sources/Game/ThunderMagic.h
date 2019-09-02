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
public:
	ThunderMagic(MagicManager* magicManager);
	~ThunderMagic();

public:
	// �����@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �����@���J������
	void Lost() override;
	// �����@�𐶐�����
	void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) override;
	// �����@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const SphereCollider& collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager*                                m_pMagicManager;
	// ���������p�^�C�}�[�쓮�t���O
	bool                                         m_startTimer;
	// ���������p�^�C�}�[
	float                                        m_time;
};


#endif // !THUNDER_MAGIC_DEFINED

