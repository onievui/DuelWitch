#include "CharaIcon.h"
#include <Utils\Resource.h>
#include <Utils\ServiceLocater.h>
#include <Utils\AudioManager.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="layer">レイヤー</param>
/// <param name="pos">座標</param>
/// <param name="pTexture">テクスチャへのポインタ</param>
/// <param name="textureIndex">テクスチャのインデックス</param>
CharaIcon::CharaIcon(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::CharaIcon, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex) {
	// テクスチャのサイズを読み込む
	if (pTexture) {
		m_size = pTexture->GetSize();
	}
}

/// <summary>
/// マウス侵入時処理
/// </summary>
void CharaIcon::OnMouseEnter() {
	// 効果音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Cursor);
}

/// <summary>
/// マウスオーバー時処理
/// </summary>
void CharaIcon::OnMouseOver() {
	// 一時的にイベントIDをマウスオーバーに変えてから通知する
	UIEventID tmpID = m_eventID;
	m_eventID = UIEventID::MouseOver;
	Notify();
	m_eventID = tmpID;
}

/// <summary>
/// クリック時処理
/// </summary>
void CharaIcon::OnClick() {
	UISubject::OnClick();
	// 効果音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Decision);
}
