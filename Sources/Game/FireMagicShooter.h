#pragma once
#ifndef FIRE_MAGIC_SHOOTER_DEFINED
#define FIRE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// �����@���˃N���X
/// </summary>
class FireMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	FireMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// �����@�𔭎˂���
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// �����������@�̓o�^��ւ̃|�C���^
	std::vector<IMagic*>* m_pMagics;
	// ���@�t�@�N�g���ւ̃|�C���^
	MagicFactory* m_pMagicFactory;

};


#endif // !FIRE_MAGIC_SHOOTER_DEFINED


