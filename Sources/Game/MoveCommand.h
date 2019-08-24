#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadDataHolder.h>


/// <summary>
/// �ړ��R�}���h�N���X
/// </summary>
class MoveCommand : public Command {
public:
	// �R���X�g���N�^
	MoveCommand() : m_totalElapsedTime(), m_euler() {}

public:
	// �ړ��R�}���h����������
	void Execute(Player& player, const DX::StepTimer& timer) override;

private:
	// ���o�ߎ���
	float                        m_totalElapsedTime;
	// �I�C���[�p
	DirectX::SimpleMath::Vector3 m_euler;

private:
	/// <summary>
	/// �ړ��R�}���h�N���X�p�p�����[�^
	/// </summary>
	class MoveCommandData {
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
	};

	// �ǂݍ��݃f�[�^�ێ��I�u�W�F�N�g
	static LoadDataHolder<MoveCommandData, LoadDataID::PlayScene> s_data;
};


#endif // !MOVE_COMMAND_DEFINED
