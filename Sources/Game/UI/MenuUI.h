#pragma once
#ifndef MENU_UI_DEFINED
#define MENU_UI_DEFINED


#include <Utils\ScaleUpUI.h>
enum class SoundID;


/// <summary>
/// メニュー用UI
/// </summary>
class MenuUI : public ScaleUpUI {
public:
	// コンストラクタ
	MenuUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);
		
public:
	// UIを更新する
	void Update(const DX::StepTimer& timer) override;

protected:
	// マウス侵入時処理
	void OnMouseEnter() override;
	// クリック時処理
	void OnClick() override;

public:
	// UIを選択する
	void Select();
	// UIを選択解除する
	void Deselect()                       { m_isSelected = false; }
	// クリック時処理を呼び出す
	void Decide()                         { OnClick(); }
	// マウス侵入時の効果音を設定する
	void SetOnMouseEnterSound(SoundID id) { m_onMouseEnterSoundId = id; }
	// クリック時の効果音を設定する
	void SetOnClickSound(SoundID id)      { m_onClickSoundId = id; }

protected:
	// UIが選択されているかどうか
	bool                  m_isSelected;
	// マウス侵入時の効果音
	SoundID               m_onMouseEnterSoundId;
	// クリック時の効果音
	SoundID               m_onClickSoundId;
};

#endif // !MENU_UI_DEFINED
