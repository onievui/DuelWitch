#pragma once
#ifndef EVENT_LOGGER_DEFINED
#define EVENT_LOGGER_DEFINED


#include <list>
#include <Framework\StepTimer.h>
#include "EventLogID.h"


/// <summary>
/// �C�x���g���K�[
/// </summary>
class EventLogger {
private:
	/// <summary>
	/// �C�x���g���O
	/// </summary>
	class EventLog {
	public:
		// ���O�̕\������
		static constexpr float LIFE_TIME = 3.0f;
		// ���O�̈ړ�����
		static constexpr float MOVE_TIME = 0.25f;
		// ���O�������̂ɂ����鎞��
		static constexpr float FADE_TIME = 0.5f;
		// �t���[���̍��W�I�t�Z�b�g
		static constexpr float POS_OFFSET_X = -450.0f;
		static constexpr float POS_OFFSET_Y = 30.0f;
		// ���O�̔z�u�Ԋu
		static constexpr float INTERVAL = 150.0f;
		// �t���[���ƃ��O�̍��W�̃I�t�Z�b�g
		static constexpr float RENDER_OFFSET = 10.0f;
		// �t���[���摜�̕s�����x
		static constexpr float FRAME_ALPHA = 0.5f;
		// ���O�摜�̕s�����x
		static constexpr float LOG_ALPHA = 0.8f;

	public:
		// �R���X�g���N�^
		EventLog(EventLogID id);
		EventLog(EventLogID id, const DirectX::SimpleMath::Vector2& prevPos);

	public:
		// �C�x���g���O���X�V����
		void Update(const DX::StepTimer& timer);
		// �C�x���g���O��`�悷��
		void Render(DirectX::SpriteBatch* spriteBatch) const;

	public:
		// ���W���擾����
		const DirectX::SimpleMath::Vector2& GetPos() const { return m_pos; }
		// ���O�����������m�F����
		bool IsDead() const;
		// �ʒu�C���f�b�N�X�����炷
		void ShiftPosIndex(int shift = 1);

	private:
		// �C�x���g���OID
		EventLogID                   m_id;
		// �ʒu�C���f�b�N�X
		int                          m_posIndex;
		// �o�^����Ă���̎���
		float                        m_time;
		// ���W
		DirectX::SimpleMath::Vector2 m_pos;

	};

public:
	// �R���X�g���N�^
	EventLogger();

public:
	// �C�x���g���K�[���X�V����
	void Update(const DX::StepTimer& timer);
	// �C�x���g���O��`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// �C�x���g���O��o�^����
	void Log(EventLogID id);

private:
	// �C�x���g���O
	std::list<std::unique_ptr<EventLog>> m_logs;

};


#endif // !EVENT_LOGGER_DEFINED
