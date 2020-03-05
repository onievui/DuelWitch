#pragma once
#ifndef UI_EVENT_DEFINED
#define UI_EVENT_DEFINED


/// <summary>
/// UIイベントID
/// 値が大きいほど優先度が高い
/// </summary>
enum class UIEventID {
	Null,
	MouseOver,
	Title,
	CharaSelect,
	Tutorial,
	Play,
	Resume,
	Option,
	Exit,
	Back,
	Next,
	CharaIcon,
};


class UISubject;


/// <summary>
/// UIイベント構造体
/// </summary>
struct UIEvent {
	UIEventID                    eventID;
	int                          layer;
	const UISubject*             address;
	DirectX::SimpleMath::Vector2 pos;
};


#endif // !UI_EVENT_DEFINED
