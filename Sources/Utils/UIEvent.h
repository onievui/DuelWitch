#pragma once
#ifndef UI_EVENT_DEFINED
#define UI_EVENT_DEFINED


/// <summary>
/// UIイベントID
/// </summary>
enum class UIEventID {
	Null,
	Title,
	CharaSelect,
	Tutorial,
	Play,
	Resume,
	Option,
	Exit,
	Back,
	Next,
	MouseOver,
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
