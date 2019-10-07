#pragma once
#ifndef NORAML_MAGIC_SHOOTER_DEFINED
#define NORAML_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;


/// <summary>
/// �ʏ햂�@���˃N���X
/// </summary>
class NormalMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	NormalMagicShooter(MagicManager* pMagicManager);

public:
	// �ʏ햂�@�𔭎˂���
	void Create(MagicFactory* magicFactory, const MagicInfo& magicInfo,
		const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager* m_pMagicManager;

};


#endif // !NORMAL_MAGIC_SHOOTER_DEFINED


