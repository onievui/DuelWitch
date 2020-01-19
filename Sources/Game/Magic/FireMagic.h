#pragma once
#ifndef FIRE_MAGIC_DEFINED
#define FIRE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class IEffectEmitter;


/// <summary>
/// �����@�N���X
/// </summary>
class FireMagic : public Magic {
public:
	// �R���X�g���N�^
	FireMagic();
	// �f�X�g���N�^
	~FireMagic();

public:
	// �����@�𐶐�����
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �����@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �����@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// �����@�̏I���������s��
	void Finalize() override;

public:
	// �_���[�W���擾����
	float GetPower() const override;
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const Collider* collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// �����@�G�t�F�N�g
	IEffectEmitter* m_pEffect;

};


#endif // !FIRE_MAGIC_DEFINED

