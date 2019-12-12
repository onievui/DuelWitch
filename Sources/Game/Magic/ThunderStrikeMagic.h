#pragma once
#ifndef THUNDER_STRIKE_MAGIC_DEFINED
#define THUNDER_STRIKE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class IEffectEmitter;


/// <summary>
/// �������@�N���X
/// </summary>
class ThunderStrikeMagic : public Magic {
public:
	// �R���X�g���N�^
	ThunderStrikeMagic();
	// �f�X�g���N�^
	~ThunderStrikeMagic();

public:
	// �������@�𐶐�����
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �������@���X�V����
	void Update(const DX::StepTimer& timer) override;
	// �������@���J������
	void Lost() override;
	// �������@��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	// �������@�̏I���������s��
	void Finalize() override;

public:
	// �_���[�W���擾����
	float GetPower() const override;
	// �v���C���[�Ƃ̏Փˏ���
	void HitPlayer(const Collider* collider) override;
	// ���@�Ƃ̏Փˏ���
	void HitMagic(const IMagic* other) override;

private:
	// �������@�G�t�F�N�g
	IEffectEmitter* m_pEffect;

};


#endif // !THUNDER_STRIKE_STRIKE_MAGIC_DEFINED
