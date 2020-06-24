#include "MenuUI.h"
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>
#include <Utils\AudioManager.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="eventID">イベントID</param>
/// <param name="layer">レイヤー</param>
/// <param name="pos">座標</param>
/// <param name="size">サイズ</param>
/// <param name="pTexture">テクスチャへのポインタ</param>
/// <param name="textureIndex">テクスチャのインデックス</param>
MenuUI::MenuUI(UIEventID eventID, int layer,
	const DirectX::SimpleMath::Vector2& pos, const DirectX::SimpleMath::Vector2& size,
	const TextureResource* pTexture, int textureIndex)
	: ScaleUpUI(eventID, layer, pos, size, pTexture, textureIndex)
	, m_isSelected(false)
	, m_onMouseEnterSoundId(SoundID::Cursor)
	, m_onClickSoundId(SoundID::Decision) {
}

/// <summary>
/// UIを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void MenuUI::Update(const DX::StepTimer& timer) {
	// パッド未接続の場合
	if (!ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		UISubject::Update(timer);
		// マウスオーバー時に拡大する
		m_isScaleUp = m_onMouseOver;
	}
	// パッド接続の場合
	else {
		m_isScaleUp = m_isSelected;
		// ボタンを押したときにクリック判定を取る
		const bool button_pressed = ServiceLocater<InputManager>::Get()->IsPressed(InputID::Desicion);
		if (m_isSelected && button_pressed) {
			OnClick();
		}
	}

	// タイマーを進める
	if (m_clickWaitTime > 0) {
		float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
		m_clickWaitTime -= elapsed_time;
	}
	
}

/// <summary>
/// マウス侵入時処理
/// </summary>
void MenuUI::OnMouseEnter() {
	// 効果音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onMouseEnterSoundId);
	m_textureIndex = 1;
}

/// <summary>
/// マウス離脱時処理
/// </summary>
void MenuUI::OnMouseExit() {
	m_textureIndex = 0;
}

/// <summary>
/// クリック時処理
/// </summary>
void MenuUI::OnClick() {
	ScaleUpUI::OnClick();
	// 効果音を鳴らす処理を追加する
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onClickSoundId);
}

/// <summary>
/// UIを選択する
/// </summary>
void MenuUI::Select() {
	m_isSelected = true;
	// パッド接続時は選択したときにマウス侵入判定を取る
	if (ServiceLocater<InputManager>::Get()->IsPadConnected()) {
		OnMouseEnter();
	}
}
