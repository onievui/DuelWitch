#pragma once
#ifndef UI_EVENT_DEFINED
#define UI_EVENT_DEFINED


/// <summary>
/// UI�C�x���gID
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
/// UI�C�x���g�\����
/// </summary>
struct UIEvent {
	UIEventID                    eventID;
	int                          layer;
	const UISubject*             address;
	DirectX::SimpleMath::Vector2 pos;
};


#endif // !UI_EVENT_DEFINED
