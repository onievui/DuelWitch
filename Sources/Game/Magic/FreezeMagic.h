#pragma once
#ifndef FREEZE_MAGIC_DEFINED
#define FREEZE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


/// <summary>
/// �X���@�N���X
/// </summary>
class FreezeMagic : public Magic {
public:
	// �R���X�g���N�^
	FreezeMagic();
	// �f�X�g���N�^
	~FreezeMagic();

public:
	// �X���@�𐶐�����
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �X���@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �X���@���J������
	void Lost() override;
	// �X���@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// �_���[�W���擾����
	float GetPower() const override;
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const Collider* collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// �v���C���[�̍��W
	const DirectX::SimpleMath::Vector3*          m_pPlayerPos;

};


#endif // !FREEZE_MAGIC_DEFINED

