#include "EventLogger.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\Resource.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="id">�C�x���g���OID</param>
EventLogger::EventLog::EventLog(EventLogID id)
	// ��O�̃��O�̈ʒu�����ō쐬����
	: EventLog(id, DirectX::SimpleMath::Vector2(ServiceLocater<DirectX11>::Get()->GetWidth() + POS_OFFSET_X, POS_OFFSET_Y)) {
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="id">�C�x���g���OID</param>
/// <param name="prevPos">��O�̃��O�̍��W</param>
EventLogger::EventLog::EventLog(EventLogID id, const DirectX::SimpleMath::Vector2& prevPos)
	: m_id(id)
	, m_posIndex()
	, m_time()
	, m_pos(prevPos.x, prevPos.y - INTERVAL) {
}

void EventLogger::EventLog::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// ���O�̂���ׂ��ʒu�Ɍ�����
	float target_pos = POS_OFFSET_Y + INTERVAL * m_posIndex;
	bool can_move = (m_pos.y < target_pos);
	if (can_move) {
		// �ʒu��������x�߂��ꍇ�͏��X�ɒx��������
		constexpr float smooth = 0.0f;
		if (target_pos - m_pos.y < INTERVAL*smooth) {
			float t1 = m_time + (target_pos - m_pos.y) / (INTERVAL / MOVE_TIME);
			float t0 = t1 - (INTERVAL*smooth) / (INTERVAL / MOVE_TIME);
			float t = (m_time - t0) / (t1 - t0);
			t = t * (2 - t);
			m_pos.y = Math::Lerp(target_pos-INTERVAL* smooth, target_pos, t);
		}
		// �����ꍇ�͓����œ�����
		else {
			m_pos.y = std::min(m_pos.y + INTERVAL / MOVE_TIME * elapsed_time, target_pos);
		}
	}

	// �\�����Ԃ��߂����������艺�ɓ�����
	if (m_time > LIFE_TIME && !can_move) {
		m_pos.y += elapsed_time * (INTERVAL*0.1f);
	}
}

/// <summary>
/// �C�x���g���O��`�悷��
/// </summary>
/// <param name="spriteBatch"></param>
void EventLogger::EventLog::Render(DirectX::SpriteBatch* spriteBatch) const {
	// �����Ă���r���͕s�����x��������
	float alpha_rate = 1.0f - (std::max(m_time - LIFE_TIME, 0.0f) / FADE_TIME);
	// �t���[���̕`��
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::EventLogFrame);
	spriteBatch->Draw(texture->GetResource().Get(), m_pos, DirectX::SimpleMath::Color(1, 1, 1, FRAME_ALPHA*alpha_rate));

	// ���O�̕`��
	texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::EventLog);
	spriteBatch->Draw(texture->GetResource(static_cast<int>(m_id)).Get(),
		m_pos + DirectX::SimpleMath::Vector2::One*RENDER_OFFSET,
		DirectX::SimpleMath::Color(1, 1, 1, LOG_ALPHA*alpha_rate));
}

/// <summary>
/// ���O�����������m�F����
/// </summary>
/// <returns>
/// true : �����Ă���
/// false : �����Ă��Ȃ�
/// </returns>
bool EventLogger::EventLog::IsDead() const {
	return (m_time > LIFE_TIME + FADE_TIME);
}

/// <summary>
/// �ʒu�C���f�b�N�X�����炷
/// </summary>
/// <param name="shift">���炷��</param>
void EventLogger::EventLog::ShiftPosIndex(int shift) {
	m_posIndex += shift;
}


/// <summary>
/// �R���X�g���N�^
/// </summary>
EventLogger::EventLogger() {
	m_logs.clear();
}

/// <summary>
/// �C�x���g���K�[���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void EventLogger::Update(const DX::StepTimer& timer) {
	for (std::list<std::unique_ptr<EventLog>>::iterator itr = m_logs.begin(); itr != m_logs.end(); itr++) {
		(*itr)->Update(timer);
	}
	// ���������O�����X�g�������
	m_logs.remove_if([](const std::unique_ptr<EventLog>& log) {return log->IsDead(); });
}

/// <summary>
/// �C�x���g���O��`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void EventLogger::Render(DirectX::SpriteBatch* spriteBatch) const {
	for (std::list<std::unique_ptr<EventLog>>::const_iterator itr = m_logs.cbegin(); itr != m_logs.cend(); itr++) {
		(*itr)->Render(spriteBatch);
	}
}

/// <summary>
/// �C�x���g���O��o�^����
/// </summary>
/// <param name="id">�C�x���g���OID</param>
void EventLogger::Log(EventLogID id) {
	// �o�^�ς݂̃C�x���g������Ȃ�A���̈ʒu���Q�Ƃ��ēo�^����
	if (m_logs.size() > 0) {
		// ���ɓo�^�ς݂̃C�x���g���O�����炷
		for (std::list<std::unique_ptr<EventLog>>::iterator itr = m_logs.begin(); itr != m_logs.end(); itr++) {
			(*itr)->ShiftPosIndex();
		}
		m_logs.emplace_back(std::make_unique<EventLog>(id, m_logs.back()->GetPos()));
	}
	else {
		m_logs.emplace_back(std::make_unique<EventLog>(id));
	}
}
