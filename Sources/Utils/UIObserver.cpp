#include "UIObserver.h"
#include "ISubject.h"
#include "UISubject.h"
#include "ErrorMessage.h"


/// <summary>
/// コンストラクタ
/// </summary>
UIObserver::UIObserver()
	: m_reservedEvent()
	, m_hasNewEvent(false) {
}

/// <summary>
/// オブザーバを更新する
/// </summary>
/// <param name="subject">サブジェクト</param>
void UIObserver::Update(ISubject* subject) {
	// UIサブジェクトかどうか確認
	UISubject* ui_subject = dynamic_cast<UISubject*>(subject);
	if (!ui_subject) {
		ErrorMessage(L"UIオブザーバにUIサブジェクトでないものから通知を受けました");
		return;
	}
	// イベントを取得
	m_reservedEvent = ui_subject->GetUIEvent();
	// イベントを取得している状態にする
	m_hasNewEvent = true;
}

/// <summary>
/// イベントを取得する
/// </summary>
/// <returns>
/// イベント
/// </returns>
UIEvent UIObserver::GetEvent() {
	// 取得フラグを解除する
	m_hasNewEvent = false;
	return m_reservedEvent;
}

/// <summary>
/// イベントIDを取得する
/// </summary>
/// <returns>
/// イベントID
/// </returns>
UIEventID UIObserver::GetEventID() {
	if (m_hasNewEvent) {
		// 取得フラグを解除する
		m_hasNewEvent = false;
		return m_reservedEvent.eventID;
	}
	else {
		// 取得フラグがfalseならNullイベントを返す
		return UIEventID::Null;
	}
}
