#pragma once
#ifndef CAST_MAGIC_COMMAND_DEFINED
#define CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"


/// <summary>
/// ���@�r���R�}���h�N���X
/// </summary>
class CastMagicCommand : public Command {
public:
	// �R���X�g���N�^
	CastMagicCommand();

public:
	// ���@�r���R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���@�̂��߂̃��C�p���ʂ̍쐬
	DirectX::SimpleMath::Plane CreatePlaneForMagic(const Transform& transform, Player::MoveDirection direction);

};


#endif // !CAST_MAGIC_COMMAND_DEFINED
