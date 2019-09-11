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
	void Update(ISubject* subject);
	// イベントを取得しているか確認
	bool IsReceiving()const { return m_isReceiving; }
	// イベントを取得する
	UIEvent GetEvent();
	// イベントIDを取得する
	UIEventID GetEventID();

private:
	// イベントを取得しているかのフラグ
	bool    m_isReceiving;
	// 受け取ったイベント
	UIEvent m_reservedEvent;
};

#endif	// !UI_OBSERVER_DEFINED
