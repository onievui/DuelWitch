#pragma once
#ifndef UI_OBSERVER_DEFINED
#define UI_OBSERVER_DEFINED


#include "IObserver.h"
#include "UIEvent.h"


/// <summary>
/// UI監視オブザーバクラス
/// </summary>
class UIObserver : public IObserver {
public:
	// コンストラクタ
	UIObserver();

public:
	// オブザーバを更新する
	virtual void Update(ISubject* subject);
	// 新しいイベントを取得しているか確認
	bool HasNewEvent()const { return m_hasNewEvent; }
	// イベントを取得する
	UIEvent GetEvent();
	// イベントIDを取得する
	UIEventID GetEventID();

protected:
	// 新しいイベントを取得しているかのフラグ
	bool    m_hasNewEvent;
	// 受け取ったイベント
	UIEvent m_reservedEvent;
};

#endif	// !UI_OBSERVER_DEFINED
