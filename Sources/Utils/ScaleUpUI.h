#pragma once
#ifndef SCALE_UP_UI_DEFINED
#define SCALE_UP_UI_DEFINED


#include "UISubject.h"


/// <summary>
/// マウスが乗っている間拡大するUIクラス
/// </summary>
class ScaleUpUI : public UISubject {
public:
	// コンストラクタ
	ScaleUpUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);

public:
	// UIを更新する
	void Update(const DX::StepTimer& timer) override;
	// UIを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) const override;

protected:
	// クリック時処理
	void OnClick() override;
	// マウスオーバー時処理
	void OnMouseOver() override;
	// 待機時処理
	void OnIdle() override;

protected:
	bool  m_onMouseOver;
	float m_clickWaitTime;
};

#endif // !SCALE_UP_UI_DEFINED
