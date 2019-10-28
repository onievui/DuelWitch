#pragma once
#ifndef FREEZE_MAGIC_SHOOTER_DEFINED
#define FREEZE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// �X���@���˃N���X
/// </summary>
class FreezeMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	FreezeMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// �X���@�𔭎˂���
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// �����������@�̓o�^��ւ̃|�C���^
	std::vector<IMagic*>* m_pMagics;
	// ���@�t�@�N�g���ւ̃|�C���^
	MagicFactory* m_pMagicFactory;

};


#endif // !FREEZE_MAGIC_SHOOTER_DEFINED

