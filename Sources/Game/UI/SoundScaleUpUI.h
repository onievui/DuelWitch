#pragma once
#ifndef SOUND_SCALE_UP_UI_DEFINED
#define SOUND_SCALE_UP_UI_DEFINED


#include <Utils\ScaleUpUI.h>
enum class SoundID;


/// <summary>
/// ScaleUpUIの音付き版
/// </summary>
class SoundScaleUpUI : public ScaleUpUI {
public:
	// コンストラクタ
	SoundScaleUpUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);
		

protected:
	// マウス侵入時処理
	void OnMouseEnter() override;
	// クリック時処理
	void OnClick() override;

public:
	// マウス侵入時の効果音を設定する
	void SetOnMouseEnterSound(SoundID id) { m_onMouseEnterSoundId = id; }
	// クリック時の効果音を設定する
	void SetOnClickSound(SoundID id)      { m_onClickSoundId = id; }

protected:
	// マウス侵入時の効果音
	SoundID               m_onMouseEnterSoundId;
	// クリック時の効果音
	SoundID               m_onClickSoundId;
};

#endif // !SOUND_SCALE_UP_UI_DEFINED
