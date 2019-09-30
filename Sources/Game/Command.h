#pragma once
#ifndef COMMAND_DEFINED
#define COMMAND_DEFINED


#include <Framework\StepTimer.h>
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "MagicManager.h"


/// <summary>
/// �v���C���[�R�}���h�N���X
/// </summary>
class Command {
public:
	// �R�}���h������������
	virtual void Initialize(Player& player) { player; }
	// �R�}���h����������
	virtual void Execute(Player& player, const DX::StepTimer& timer) = 0;

protected:
	// �v���C���[�̎p�����擾����
	Transform&                   GetTransform(Player& player)                { return player.m_transform; }
	// �v���C���[�̎p�����擾����
	const Transform&             GetTransform(const Player& player) const    { return player.m_transform; }
	// �v���C���[�̍s����擾����
	DirectX::SimpleMath::Matrix& GetWorld(Player& player)                    { return player.m_world; }
	// �v���C���[�̃X�e�[�^�X���擾����
	Player::Status&              GetStatus(Player& player)                   { return player.m_status; }
	// �v���C���[�̃X�e�[�^�X���擾����
	const Player::Status&        GetStatus(const Player& player) const       { return player.m_status; }
	// �v���C���[�̐i�s�������擾����
	Player::MoveDirection&       GetMoveDirection(Player& player)            { return player.m_direction; }
	// �v���C���[�̏����G�������g���擾����
	std::list<ElementID>&        GetHaveElements(Player& player)             { return player.m_haveElements; }
	// �v���C���[�̏����G�������g���擾����
	const std::list<ElementID>&  GetHaveElements(const Player& player) const { return player.m_haveElements; }
	// �v���C���[�̓����蔻����擾����
	const SphereCollider&        GetCollider(const Player& player) const     { return player.m_sphereCollider; }
	// �J�������擾����
	Camera&                      GetCamera(Player& player)                   { return *player.m_pCamera; }
	// ���@�}�l�[�W�����擾����
	MagicManager&                GetMagicManager(Player& player)             { return *player.m_pMagicManager; }
	// �G�v���C���[���擾����
	const Player&                GetOtherPlayer(Player& player)              { return *player.m_pOtherPlayer; }
};


#endif // !COMMAND_DEFINED
