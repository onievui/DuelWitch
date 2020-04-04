#include "EventLogger.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\Resource.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="id">イベントログID</param>
EventLogger::EventLog::EventLog(EventLogID id)
	// 一つ前のログの位置を仮で作成する
	: EventLog(id, DirectX::SimpleMath::Vector2(ServiceLocater<DirectX11>::Get()->GetWidth() + POS_OFFSET_X, POS_OFFSET_Y)) {
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="id">イベントログID</param>
/// <param name="prevPos">一つ前のログの座標</param>
EventLogger::EventLog::EventLog(EventLogID id, const DirectX::SimpleMath::Vector2& prevPos)
	: m_id(id)
	, m_posIndex()
	, m_time()
	, m_pos(prevPos.x, prevPos.y - INTERVAL) {
}

void EventLogger::EventLog::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// ログのいるべき位置に向かう
	float target_pos = POS_OFFSET_Y + INTERVAL * m_posIndex;
	bool can_move = (m_pos.y < target_pos);
	if (can_move) {
		// 位置がある程度近い場合は徐々に遅く動かす
		constexpr float smooth = 0.0f;
		if (target_pos - m_pos.y < INTERVAL*smooth) {
			float t1 = m_time + (target_pos - m_pos.y) / (INTERVAL / MOVE_TIME);
			float t0 = t1 - (INTERVAL*smooth) / (INTERVAL / MOVE_TIME);
			float t = (m_time - t0) / (t1 - t0);
			t = t * (2 - t);
			m_pos.y = Math::Lerp(target_pos-INTERVAL* smooth, target_pos, t);
		}
		// 遠い場合は等速で動かす
		else {
			m_pos.y = std::min(m_pos.y + INTERVAL / MOVE_TIME * elapsed_time, target_pos);
		}
	}

	// 表示時間を過ぎたらゆっくり下に動かす
	if (m_time > LIFE_TIME && !can_move) {
		m_pos.y += elapsed_time * (INTERVAL*0.1f);
	}
}

/// <summary>
/// イベントログを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void EventLogger::EventLog::Render(DirectX::SpriteBatch* spriteBatch) const {
	// 消えている途中は不透明度を下げる
	float alpha_rate = 1.0f - (std::max(m_time - LIFE_TIME, 0.0f) / FADE_TIME);
	// フレームの描画
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::EventLogFrame);
	spriteBatch->Draw(texture->GetResource().Get(), m_pos, DirectX::SimpleMath::Color(1, 1, 1, FRAME_ALPHA*alpha_rate));

	// ログの描画
	texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::EventLog);
	spriteBatch->Draw(texture->GetResource(static_cast<int>(m_id)).Get(),
		m_pos + DirectX::SimpleMath::Vector2::One*RENDER_OFFSET,
		DirectX::SimpleMath::Color(1, 1, 1, LOG_ALPHA*alpha_rate));
}

/// <summary>
/// ログが消えたか確認する
/// </summary>
/// <returns>
/// true : 消えている
/// false : 消えていない
/// </returns>
bool EventLogger::EventLog::IsDead() const {
	return (m_time > LIFE_TIME + FADE_TIME);
}

/// <summary>
/// 位置インデックスをずらす
/// </summary>
/// <param name="shift">ずらす量</param>
void EventLogger::EventLog::ShiftPosIndex(int shift) {
	m_posIndex += shift;
}


/// <summary>
/// コンストラクタ
/// </summary>
EventLogger::EventLogger() {
	m_logs.clear();
}

/// <summary>
/// イベントロガーを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void EventLogger::Update(const DX::StepTimer& timer) {
	for (std::list<std::unique_ptr<EventLog>>::iterator itr = m_logs.begin(); itr != m_logs.end(); itr++) {
		(*itr)->Update(timer);
	}
	// 消えたログをリストから消す
	m_logs.remove_if([](const std::unique_ptr<EventLog>& log) {return log->IsDead(); });
}

/// <summary>
/// イベントログを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void EventLogger::Render(DirectX::SpriteBatch* spriteBatch) const {
	for (std::list<std::unique_ptr<EventLog>>::const_iterator itr = m_logs.cbegin(); itr != m_logs.cend(); itr++) {
		(*itr)->Render(spriteBatch);
	}
}

/// <summary>
/// イベントログを登録する
/// </summary>
/// <param name="id">イベントログID</param>
void EventLogger::Log(EventLogID id) {
	// 登録済みのイベントがあるなら、その位置を参照して登録する
	if (m_logs.size() > 0) {
		// 既に登録済みのイベントログをずらす
		for (std::list<std::unique_ptr<EventLog>>::iterator itr = m_logs.begin(); itr != m_logs.end(); itr++) {
			(*itr)->ShiftPosIndex();
		}
		m_logs.emplace_back(std::make_unique<EventLog>(id, m_logs.back()->GetPos()));
	}
	else {
		m_logs.emplace_back(std::make_unique<EventLog>(id));
	}
}
