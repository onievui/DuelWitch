#pragma once
#ifndef EVENT_LOGGER_DEFINED
#define EVENT_LOGGER_DEFINED


#include <list>
#include <Framework\StepTimer.h>
#include "EventLogID.h"


/// <summary>
/// イベントロガー
/// </summary>
class EventLogger {
private:
	/// <summary>
	/// イベントログ
	/// </summary>
	class EventLog {
	public:
		// ログの表示時間
		static constexpr float LIFE_TIME = 3.0f;
		// ログの移動時間
		static constexpr float MOVE_TIME = 0.25f;
		// ログを消すのにかかる時間
		static constexpr float FADE_TIME = 0.5f;
		// フレームの座標オフセット
		static constexpr float POS_OFFSET_X = -450.0f;
		static constexpr float POS_OFFSET_Y = 30.0f;
		// ログの配置間隔
		static constexpr float INTERVAL = 150.0f;
		// フレームとログの座標のオフセット
		static constexpr float RENDER_OFFSET = 10.0f;
		// フレーム画像の不透明度
		static constexpr float FRAME_ALPHA = 0.5f;
		// ログ画像の不透明度
		static constexpr float LOG_ALPHA = 0.8f;

	public:
		// コンストラクタ
		EventLog(EventLogID id);
		EventLog(EventLogID id, const DirectX::SimpleMath::Vector2& prevPos);

	public:
		// イベントログを更新する
		void Update(const DX::StepTimer& timer);
		// イベントログを描画する
		void Render(DirectX::SpriteBatch* spriteBatch) const;

	public:
		// 座標を取得する
		const DirectX::SimpleMath::Vector2& GetPos() const { return m_pos; }
		// ログが消えたか確認する
		bool IsDead() const;
		// 位置インデックスをずらす
		void ShiftPosIndex(int shift = 1);

	private:
		// イベントログID
		EventLogID                   m_id;
		// 位置インデックス
		int                          m_posIndex;
		// 登録されてからの時間
		float                        m_time;
		// 座標
		DirectX::SimpleMath::Vector2 m_pos;

	};

public:
	// コンストラクタ
	EventLogger();

public:
	// イベントロガーを更新する
	void Update(const DX::StepTimer& timer);
	// イベントログを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// イベントログを登録する
	void Log(EventLogID id);

private:
	// イベントログ
	std::list<std::unique_ptr<EventLog>> m_logs;

};


#endif // !EVENT_LOGGER_DEFINED
