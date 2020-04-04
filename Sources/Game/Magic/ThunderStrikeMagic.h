#pragma once
#ifndef THUNDER_STRIKE_MAGIC_DEFINED
#define THUNDER_STRIKE_MAGIC_DEFINED


#include <Framework\StepTimer.h>
#include "Magic.h"


class MagicManager;
class IEffectEmitter;


/// <summary>
/// �������@�N���X
/// </summary>
class ThunderStrikeMagic : public Magic {
public:
	// �X���@�Ɉ����񂹂���̈�̔��a
	static constexpr float ATTRACTED_RADIUS = 5.0f;
	// �X���@�Ɉ����񂹂��鑬�x
	static constexpr float ATTRACTED_SPEED = 0.5f;

public:
	// �R���X�g���N�^
	ThunderStrikeMagic(MagicManager* pMagicManager);
	// �f�X�g���N�^
	~ThunderStrikeMagic();

public:
	// �������@�𐶐�����
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �������@���X�V����
	void Update(const DX::StepTimer& timer) override;
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
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager*   m_pMagicManager;
	// �������@�G�t�F�N�g
	IEffectEmitter* m_pEffect;
	// ���˕Ԃ�����
	int             m_refrectCount;

};


#endif // !THUNDER_STRIKE_STRIKE_MAGIC_DEFINED
