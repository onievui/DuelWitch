#include "ScaleUpUI.h"
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
ScaleUpUI::ScaleUpUI(UIEventID eventID, int layer, const DirectX::SimpleMath::Vector2& pos,
	const DirectX::SimpleMath::Vector2& size, const TextureResource* pTexture, int textureIndex)
	: UISubject(eventID, layer, pos, size, pTexture, textureIndex)
	, m_isScaleUp(false)
	, m_clickWaitTime() {
}

/// <summary>
/// UIを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ScaleUpUI::Update(const DX::StepTimer& timer) {
	UISubject::Update(timer);
	// タイマーを進める
	if (m_clickWaitTime > 0) {
		float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
		m_clickWaitTime -= elapsed_time;
	}
	// マウスオーバー時に拡大する
	m_isScaleUp = m_onMouseOver;
}

/// <summary>
/// UIを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void ScaleUpUI::Render(DirectX::SpriteBatch* spriteBatch) const {
	// マウスが乗っている間は拡大する
	DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2::One*(m_isScaleUp ? 1.2f : 1.0f);
	// マウスオーバー時は赤色、クリック時は灰色にする
	DirectX::SimpleMath::Color color = DirectX::Colors::White;
	if (m_isScaleUp) {
		if (m_clickWaitTime > 0.0f) {
			color = DirectX::Colors::Gray;
		}
		else {
			color = DirectX::Colors::White;
		}
	}
	color.w *= m_alpha;

	// テクスチャがあれば描画する
	if (m_pTexture) {
		spriteBatch->Draw(m_pTexture->GetResource(m_textureIndex).Get(),
			m_pos, nullptr, color, 0, m_pTexture->GetCenter(m_textureIndex), scale);
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
		DirectX::SimpleMath::Vector2 pos(m_pos.x - rect.right*0.5f*scale.x, m_pos.y - rect.bottom*0.5f*scale.y);
		color = m_textColor;
		color.w *= m_alpha;

		font->DrawString(spriteBatch, m_text.c_str(), pos, color, 0.0f,
			DirectX::SimpleMath::Vector2::Zero, scale);
	}
}

/// <summary>
/// クリック時処理
/// </summary>
void ScaleUpUI::OnClick() {
	// 直前にクリックしていなければ処理する
	if (m_clickWaitTime <= 0.0f) {
		Notify();
		m_clickWaitTime = 0.1f;
	}
}
