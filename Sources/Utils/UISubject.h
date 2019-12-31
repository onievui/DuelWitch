#pragma once
#ifndef UI_SUBJECT_DEFINED
#define UI_SUBJECT_DEFINED


#include <Framework\StepTimer.h>
#include "Subject.h"
#include "UIEvent.h"


class TextureResource;
class FontResource;


/// <summary>
/// UIサブジェクトクラス
/// </summary>
class UISubject : public Subject {
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
	const DirectX::SimpleMath::Vector2& GetPos() const             { return m_pos; }
	// 座標を設定する
	void SetPos(const DirectX::SimpleMath::Vector2& pos)           { m_pos = pos; }
	// サイズを取得する
	const DirectX::SimpleMath::Vector2& GetSize() const            { return m_size; }
	// サイズを設定する
	void SetSize(const DirectX::SimpleMath::Vector2& size)         { m_size = size; }
	// スケールを取得する
	const DirectX::SimpleMath::Vector2& GetScale() const           { return m_scale; }
	// スケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector2& scale);
	// レイヤーを取得する
	int GetLayer() const                                           { return m_layer; }
	// レイヤーを設定する
	void SetLayer(int layer)                                       { m_layer = layer; }
	// テクスチャを取得する
	const TextureResource* GetTexture() const                      { return m_pTexture; }
	// テクスチャを設定する
	void SetTexture(const TextureResource* pTexture)               { m_pTexture = pTexture; }
	// テクスチャのインデックスを取得する
	int GetTextureIndex() const                                    { return m_textureIndex; }
	// テクスチャのインデックスを設定する
	void SetTextureIndex(int textureIndex)                         { m_textureIndex = textureIndex; }
	// テキストを取得する
	const std::wstring& GetText() const                            { return m_text; }
	// テキストを設定する
	void SetText(const std::wstring& text)                         { m_text = text; }
	// テキスト色を取得する
	const DirectX::SimpleMath::Color& GetTextColor() const         { return m_textColor; }
	// テキスト色を設定する
	void SetTextColor(const DirectX::SimpleMath::Color& textColor) { m_textColor = textColor; }
	// アルファ値を設定する
	void SetAlpha(float alpha)                                     { m_alpha = alpha; }
	// フォントを取得する
	const FontResource* GetFont()const                             { return m_pFont; }
	// フォントを設定する
	void SetFont(const FontResource* pFont)                        { m_pFont = pFont; }

	// サイズをテクスチャに合わせる
	void FitTextureSize();

protected:
	// クリック時処理
	virtual void OnClick();
	// マウスオーバー時処理
	virtual void OnMouseOver() {}
	// 待機時処理
	virtual void OnIdle() {}

protected:
	// UIイベントID
	UIEventID                    m_eventID;
	// レイヤー
	int                          m_layer;
	// 中心座標
	DirectX::SimpleMath::Vector2 m_pos;
	// サイズ
	DirectX::SimpleMath::Vector2 m_size;
	// スケール
	DirectX::SimpleMath::Vector2 m_scale;
	// テクスチャ
	const TextureResource*       m_pTexture;
	// テクスチャのインデックス
	int                          m_textureIndex;
	// テキスト
	std::wstring                 m_text;
	// テキスト色
	DirectX::SimpleMath::Color   m_textColor;
	// アルファ値
	float                        m_alpha;
	// フォント
	const FontResource*          m_pFont;
};

#endif // !UI_SUBJECT_DEFINED
