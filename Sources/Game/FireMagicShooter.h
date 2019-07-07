#pragma once
#ifndef FIRE_MAGIC_SHOOTER_DEFINED
#define FIRE_MAGIC_SHOOTER_DEFINED


#include "IMagicShooter.h"


class MagicManager;
enum class PlayerID;


/// <summary>
/// ���@���˃N���X
/// </summary>
class FireMagicShooter : public IMagicShooter {
public:
	// �R���X�g���N�^
	FireMagicShooter(MagicManager* magicManager);

public:
	// ���@���X�V����
	void Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;

private:
	// ���@�}�l�[�W���ւ̃|�C���^
	MagicManager* m_pMagicManager;

};


#endif // !FIRE_MAGIC_SHOOTER_DEFINED


