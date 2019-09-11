#pragma once
#ifndef UI_SUBJECT_DEFINED
#define UI_SUBJECT_DEFINED


#include <Framework\StepTimer.h>
#include "UIEvent.h"


class TextureResource;
class FontResource;


/// <summary>
/// UIサブジェクトクラス
/// </summary>
class UISubject {
public:
	// コンストラクタ
	UISubject(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);

public:
	// UIを更新する
	virtual void Update(const DX::StepTimer& timer);
	// UIを描画する
	virtual void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// UIイベントを取得する
	UIEvent GetUIEvent() const;
	// 座標を取得する
	const DirectX::SimpleMath::Vector2& GetPos() const { return m_pos; }
	// 座標を設定する
	void SetPos(const DirectX::SimpleMath::Vector2& pos) { m_pos = pos; }
	// サイズを取得する
	const DirectX::SimpleMath::Vector2& GetSize() const { return m_size; }
	// サイズを設定する
	void SetSize(const DirectX::SimpleMath::Vector2& size) { m_size = size; }
	// レイヤーを取得する
	int GetLayer() const { return m_layer; }
	// レイヤーを設定する
	void SetLayer(int layer) { m_layer = layer; }
	// テクスチャを設定する
	void SetTexture(const TextureResource* pTexture) { m_pTexture = pTexture; }
	// テキストを取得する
	const std::wstring& GetText() const { return m_text; }
	// テキストを設定する
	void SetText(const std::wstring& text) { m_text = text; }
	// テキスト色を取得する
	const DirectX::SimpleMath::Color& GetTextColor() const { return m_textColor; }
	// テキスト色を設定する
	void SetTextColor(const DirectX::SimpleMath::Color& textColor) { m_textColor = textColor; }
	// フォントを設定する
	void SetFont(const FontResource* pFont) { m_pFont = pFont; }

	// サイズをテクスチャに合わせる
	void FitTextureSize();

private:
	// クリック時処理
	virtual void OnClick() {}
	// マウスオーバー時処理
	virtual void OnMouseOver() {}

private:
	// UIイベントID
	UIEventID                    m_eventID;
	// レイヤー
	int                          m_layer;
	// 中心座標
	DirectX::SimpleMath::Vector2 m_pos;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;
	// テクスチャ
	const TextureResource*       m_pTexture;
	// テクスチャのインデックス
	int                          m_textureIndex;
	// テキスト
	std::wstring                 m_text;
	// テキスト色
	DirectX::SimpleMath::Color   m_textColor;
	// フォント
	const FontResource*          m_pFont;
};

#endif // !UI_SUBJECT_DEFINED
