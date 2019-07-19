#pragma once
#ifndef FREEZE_MAGIC_SHOOTER_DEFINED
#define FREEZE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// �X���@���˃N���X
/// </summary>
class FreezeMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	FreezeMagicShooter(MagicManager* magicManager);

public:
	// �X���@�𔭎˂���
	void Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager* m_pMagicManager;

};


#endif // !FREEZE_MAGIC_SHOOTER_DEFINED

