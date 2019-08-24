#pragma once
#ifndef MOVE_COMMAND_DEFINED
#define MOVE_COMMAND_DEFINED


#include "Command.h"
#include <Utils\LoadData.h>


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
	/// �ړ��R�}���h�N���X�f�[�^
	/// </summary>
	class MoveCommandData : public LoadData {
	public:
		// �R���X�g���N�^
		MoveCommandData() : LoadData(LoadDataID::PlayScene), m_value() {}

	public:
		// �f�[�^��ǂݍ���
		bool Load() override;
		void Dispose() override;

	private:
		// �ǂݍ��݃f�[�^�{��
		struct _value {
			float moveSpeed;
			float moveSpeedXY;
			float rotSpeed;
			float rotZLimit;
			float rotXLimit;
			float rotYLimit;
			float lerpSpeed;
		};

	public:
		_value* operator->() {
			return m_value.get();
		}

	private:
		// �ǂݍ��݃f�[�^�{��
		std::unique_ptr<_value> m_value;

	};

	// �ǂݍ��݃f�[�^
	static MoveCommandData s_data;
};


#endif // !MOVE_COMMAND_DEFINED
