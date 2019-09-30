#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


class PlayerTrailEffectEmitter;


/// <summary>
/// AI�ړ��R�}���h�N���X
/// </summary>
class AIMoveCommand : public Command {
public:
	// �R���X�g���N�^
	AIMoveCommand();

public:
	// AI�ړ��R�}���h������������
	void Initialize(Player& player) override;
	// AI�ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// �I�C���[�p
	DirectX::SimpleMath::Vector3            m_euler;
	//�@�v���C���[�̋O�ՃG�t�F�N�g�ւ̃|�C���^
	PlayerTrailEffectEmitter*               m_pEffect;
	// �G�t�F�N�g�p�̎p���N���X
	ChildTransform                          m_effectTransform;
};


#endif // !AI_MOVE_COMMAND_DEFINED
