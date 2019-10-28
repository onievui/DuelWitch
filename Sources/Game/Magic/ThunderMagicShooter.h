#pragma once
#ifndef THUNDER_MAGIC_SHOOTER_DEFINED
#define THUNDER_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// �����@���˃N���X
/// </summary>
class ThunderMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	ThunderMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// �����@�𔭎˂���
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// �����������@�̓o�^��ւ̃|�C���^
	std::vector<IMagic*>* m_pMagics;
	// ���@�t�@�N�g���ւ̃|�C���^
	MagicFactory* m_pMagicFactory;

};


#endif // !THUNDER_MAGIC_SHOOTER_DEFINED

