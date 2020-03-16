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
	// �R���X�g���N�^
	ThunderMagic(MagicManager* pMagicManager);
	// �f�X�g���N�^
	~ThunderMagic();

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
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager*                                m_pMagicManager;
	// ���b�N�I���p�^�C�}�[
	float                                        m_lockOnTimer;
	// ���������p�^�C�}�[�쓮�t���O
	bool                                         m_startTimer;
	// ���������p�^�C�}�[
	float                                        m_time;

};


#endif // !THUNDER_MAGIC_DEFINED

