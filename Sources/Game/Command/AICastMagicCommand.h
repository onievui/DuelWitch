#pragma once
#ifndef AI_CAST_MAGIC_COMMAND_DEFINED
#define AI_CAST_MAGIC_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// AI���@�r���R�}���h�N���X
/// </summary>
class AICastMagicCommand : public Command {
public:
	// AI���@�r���R�}���h����������State::
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// �ł��߂��G�v���C���[���擾����
	const Player* GetNearestPlayer(const DirectX::SimpleMath::Vector3& pos, const std::vector<Player*>& otherPlayers, float* distance);

private:
	// ���̍U���܂ł̑҂�����
	float m_waitTime;

};


#endif // !AI_CAST_MAGIC_COMMAND_DEFINED
