#pragma once
#ifndef AI_MOVE_COMMAND_DEFINED
#define AI_MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// AI�ړ��R�}���h�N���X
/// </summary>
class AIMoveCommand : public Command {
public:
	// �R���X�g���N�^
	AIMoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// AI�ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���o�ߎ���
	float                        m_totalElapsedTime;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3 m_euler;

private:
	/// <summary>
	/// AI�ړ��R�}���h�N���X�p�p�����[�^
	/// </summary>
	class AIMoveCommandData {
	public:
		// �f�[�^��ǂݍ���
		bool Load();

	public:
		float moveSpeed;
		float moveSpeedXY;
		float rotSpeed;
		float rotZLimit;
		float rotXLimit;
		float rotYLimit;
		float lerpSpeed;
		float nearDistance;
	};

	// �ǂݍ��݃f�[�^�ێ��I�u�W�F�N�g
	static LoadDataHolder<AIMoveCommandData, LoadDataID::PlayScene> s_data;
};


#endif // !AI_MOVE_COMMAND_DEFINED
