#include "CharaIcon.h"
#include <Utils\Resource.h>
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>



/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="layer">レイヤー</param>
/// <param name="pos">座標</param>
/// <param name="pTexture">テクスチャへのポインタ</param>
/// <param name="textureIndex">テクスチャのインデックス</param>
CharaIcon::CharaIcon(int layer, const DirectX::SimpleMath::Vector2& pos, const TextureResource* pTexture, int textureIndex)
	: UISubject(UIEventID::CharaIcon, layer, pos, DirectX::SimpleMath::Vector2(100, 100), pTexture, textureIndex)
	, m_isSelected(false) {
	// テクスチャのサイズを読み込む
	if (pTexture) {
		m_size = pTexture->GetSize();
	}
}

/// <summary>
/// UIを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void CharaIcon::Update(const DX::StepTimer& timer) {
	// パッド未接続の場合
	if (!ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		UISubject::Update(timer);
	}
	// パッド接続で選択中の場合
	else if (m_isSelected) {
		// ボタンを押したときにクリック判定を取る
		const bool button_pressed = ServiceLocater<InputManager>::Get()->IsPressed(InputID::Desicion);
		if (button_pressed) {
			OnClick();
		}
		// それ以外はマウスオーバーしていると扱う
		else {
			OnMouseOver();
		}
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

/// <summary>
/// UIを選択する
/// </summary>
void CharaIcon::Select() {
	m_isSelected = true;
	// パッド接続時は選択したときにマウス侵入判定を取る
	if (ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		OnMouseEnter();
	}
}
