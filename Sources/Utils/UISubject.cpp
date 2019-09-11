#include "UISubject.h"
#include "ServiceLocater.h"
#include "MouseWrapper.h"
#include "ErrorMessage.h"
#include "Resource.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="eventID">イベントID</param>
/// <param name="layer">レイヤー</param>
/// <param name="pos">座標</param>
/// <param name="size">サイズ</param>
/// <param name="pTexture">テクスチャへのポインタ</param>
/// <param name="textureIndex">テクスチャのインデックス</param>
UISubject::UISubject(UIEventID eventID, int layer, const DirectX::SimpleMath::Vector2& pos,
	const DirectX::SimpleMath::Vector2& size, const TextureResource* pTexture, int textureIndex)
	: m_eventID(eventID)
	, m_layer(layer)
	, m_pos(pos)
	, m_size(size)
	, m_pTexture(pTexture)
	, m_textureIndex(textureIndex)
	, m_text()
	, m_textColor(DirectX::Colors::White)
	, m_pFont() {
}

/// <summary>
/// UIを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void UISubject::Update(const DX::StepTimer& timer) {
	timer;
	MouseWrapper* mouseWrapper = ServiceLocater<MouseWrapper>::Get();
	const DirectX::SimpleMath::Vector2& mouse_pos = mouseWrapper->GetPos();
	// UIの領域内かどうか
	if (mouse_pos.x - m_pos.x < m_size.x*0.5f &&
		mouse_pos.y - m_pos.y < m_size.y*0.5f) {
		// クリックしたかどうか
		if (mouseWrapper->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			OnClick();
		}
		else {
			OnMouseOver();
		}
	}
}

/// <summary>
///	UIを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void UISubject::Render(DirectX::SpriteBatch* spriteBatch) const {
	// テクスチャがあれば描画する
	if (m_pTexture) {
		spriteBatch->Draw(m_pTexture->GetResource(m_textureIndex).Get(),
			m_pos, nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, 1), 0,
			m_pTexture->GetCenter(m_textureIndex), DirectX::SimpleMath::Vector2(1, 1));
	}
	// テキストがあれば描画する
	if (!m_text.empty()) {
		// フォントがあるか確認
		if (!m_pFont) {
			ErrorMessage(L"UIにフォントが設定されてません");
			return;
		}
		const DirectX::SpriteFont* font = m_pFont->GetResource().get();
		RECT rect = font->MeasureDrawBounds(m_text.c_str(), DirectX::SimpleMath::Vector2::Zero);
		DirectX::SimpleMath::Vector2 pos(m_pos.x - rect.right*0.5f, m_pos.y - rect.bottom*0.5f);
		font->DrawString(spriteBatch, m_text.c_str(), pos, m_textColor);
	}
}

/// <summary>
/// UIイベントを取得する
/// </summary>
/// <returns>
/// UIイベント
/// </returns>
UIEvent UISubject::GetUIEvent() const {
	UIEvent uiEvent;
	uiEvent.eventID = m_eventID;
	uiEvent.layer = m_layer;
	uiEvent.address = this;
	uiEvent.pos = m_pos;
	return uiEvent;
}

/// <summary>
/// サイズをテクスチャに合わせる
/// </summary>
void UISubject::FitTextureSize() {
	if (!m_pTexture) {
		ErrorMessage(L"UIにテクスチャが設定されていません");
		return;
	}
	m_size = m_pTexture->GetSize(m_textureIndex);
}
