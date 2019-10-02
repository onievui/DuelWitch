#pragma once
#ifndef THUNDER_MAGIC_SHOOTER_DEFINED
#define THUNDER_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// �����@���˃N���X
/// </summary>
class ThunderMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	ThunderMagicShooter(MagicManager* magicManager);

public:
	// �����@�𔭎˂���
	void Create(int level, MagicFactory* magicFactory, PlayerID playerId,
		const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager* m_pMagicManager;

};


#endif // !THUNDER_MAGIC_SHOOTER_DEFINED

