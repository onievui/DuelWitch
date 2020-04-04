#pragma once
#ifndef EVENT_LOG_ID_DEFINED
#define EVENT_LOG_ID_DEFINED


/// <summary>
/// イベントログID
/// </summary>
enum class EventLogID {
	Player1Dead,		// プレイヤー1が倒れた
	Player2Dead,		// プレイヤー2が倒れた
	Player3Dead,        // プレイヤー3が倒れた
	FieldScaleDown,     // ステージが狭くなり始めた

};


#endif // !EVENT_LOG_ID_DEFINED
