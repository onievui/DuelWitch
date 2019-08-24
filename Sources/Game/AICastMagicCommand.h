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
	// AI���@�r���R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���̍U���܂ł̑҂�����
	float m_waitTime;

private:
	/// <summary>
	/// AI���@�r���R�}���h�N���X�p�p�����[�^
	/// </summary>
	class AICastMagicCommandData {
	public:
		// �f�[�^��ǂݍ���
		bool Load();

	public:
		float shotableAngle;
		float castDelay;
	};

	// �ǂݍ��݃f�[�^�ێ��I�u�W�F�N�g
	static LoadDataHolder<AICastMagicCommandData, LoadDataID::PlayScene> s_data;

};


#endif // !AI_CAST_MAGIC_COMMAND_DEFINED
