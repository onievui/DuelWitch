#pragma once
#ifndef THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED
#define THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// �������@���˃N���X
/// </summary>
class ThunderStrikeMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	ThunderStrikeMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// �������@�𔭎˂���
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// �����������@�̓o�^��ւ̃|�C���^
	std::vector<IMagic*>* m_pMagics;
	// ���@�t�@�N�g���ւ̃|�C���^
	MagicFactory* m_pMagicFactory;

};


#endif // !THUNDER_STRIKE_MAGIC_SHOOTER_DEFINED

