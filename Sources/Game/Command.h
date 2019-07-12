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
	// �R�}���h����������
	virtual void Execute(Player& player, const DX::StepTimer& timer) = 0;

protected:
	// �v���C���[�̎p�����擾����
	Transform& GetTransform(Player& player) {
		return player.m_transform;
	}
	// �v���C���[�̎p�����擾����
	const Transform& GetTransform(const Player& player) {
		return player.m_transform;
	}
	// �v���C���[�̍s����擾����
	DirectX::SimpleMath::Matrix& GetWorld(Player& player) {
		return player.m_world;
	}
	// �v���C���[�̐i�s�������擾����
	Player::MoveDirection& GetMoveDirection(Player& player) {
		return player.m_direction;
	}
	// �J�������擾����
	Camera& GetCamera(Player& player) {
		return *player.m_pCamera;
	}
	// ���@�}�l�[�W�����擾����
	MagicManager& GetMagicManager(Player& player) {
		return *player.m_pMagicManager;
	}
	// �G�v���C���[���擾����
	const Player& GetOtherPlayer(Player& player) {
		return *player.m_otherPlayer;
	}
};


#endif // !COMMAND_DEFINED
