#pragma once
#ifndef NORAML_MAGIC_SHOOTER_DEFINED
#define NORAML_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class IMagic;


/// <summary>
/// �ʏ햂�@���˃N���X
/// </summary>
class NormalMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	NormalMagicShooter(std::vector<IMagic*>* magics, MagicFactory* magicFactory);

public:
	// �ʏ햂�@�𔭎˂���
	void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// �����������@�̓o�^��ւ̃|�C���^
	std::vector<IMagic*>* m_pMagics;
	// ���@�t�@�N�g���ւ̃|�C���^
	MagicFactory* m_pMagicFactory;

};


#endif // !NORMAL_MAGIC_SHOOTER_DEFINED


