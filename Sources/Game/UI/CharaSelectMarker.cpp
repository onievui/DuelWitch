#include "CharaSelectMarker.h"
#include <Utils\ErrorMessage.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="layer">レイヤー</param>
/// <param name="pos">座標</param>
/// <param name="pTexture">テクスチャへのポインタ</param>
/// <param name="textureIndex">テクスチャのインデックス</param>
CharaSelectMarker::CharaSelectMarker(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::Null, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex)
	, m_enableObserver(true) {

}

/// <summary>
/// オブザーバを更新する
/// </summary>
/// <param name="subject">サブジェクト</param>
void CharaSelectMarker::Update(ISubject* subject) {
	// オブザーバの機能が無効なら処理しない
	if (!m_enableObserver) {
		return;
	}

	// UIサブジェクトかどうか確認
	UISubject* ui_subject = dynamic_cast<UISubject*>(subject);
	if (!ui_subject) {
		ErrorMessage(L"キャラセレクトマーカーにUIサブジェクトでないものから通知を受けました");
		return;
	}
	// イベントを取得
	m_reservedEvent = ui_subject->GetUIEvent();
	// イベントを取得している状態にする
	m_hasNewEvent = true;

	// マウスオーバーイベントを受け取った場合は座標を取得する
	if (m_reservedEvent.eventID == UIEventID::MouseOver) {
		m_pos = m_reservedEvent.pos;
	}
}
