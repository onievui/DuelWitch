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
	, m_scale(DirectX::SimpleMath::Vector2(1,1))
	, m_pTexture(pTexture)
	, m_textureIndex(textureIndex)
	, m_text()
	, m_textColor(DirectX::Colors::White)
	, m_alpha(1.0f)
	, m_pFont()
	, m_onMouseOver(false) {
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
	if (std::fabsf(mouse_pos.x - m_pos.x) < m_size.x*0.5f &&
		std::fabsf(mouse_pos.y - m_pos.y) < m_size.y*0.5f) {
		// マウスが侵入したかどうか
		if (!m_onMouseOver) {
			OnMouseEnter();
			m_onMouseOver = true;
		}
		OnMouseOver();
		// クリックしたかどうか
		if (mouseWrapper->GetTracker()->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
			OnClick();
		}
	}
	else {
		// マウスが離脱したかどうか
		if (m_onMouseOver) {
			OnMouseExit();
			m_onMouseOver = false;
		}
		OnIdle();
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
			m_pos, nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha), 0,
			m_pTexture->GetCenter(m_textureIndex), m_scale);
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
		DirectX::SimpleMath::Color color = m_textColor;
		color.w *= m_alpha;

		font->DrawString(spriteBatch, m_text.c_str(), pos, color, 0.0f,
			DirectX::SimpleMath::Vector2::Zero, m_scale);
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
/// スケールを設定する
/// </summary>
/// <param name="scale">スケール</param>
void UISubject::SetScale(const DirectX::SimpleMath::Vector2& scale) {
	m_size *= scale / m_scale;
	m_scale = scale;
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

/// <summary>
/// クリック時処理
/// </summary>
void UISubject::OnClick() {
	// オブザーバに通知する
	Notify();
}
