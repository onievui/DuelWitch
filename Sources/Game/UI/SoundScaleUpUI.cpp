#include "SoundScaleUpUI.h"
#include <Utils\ServiceLocater.h>
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
SoundScaleUpUI::SoundScaleUpUI(UIEventID eventID, int layer,
	const DirectX::SimpleMath::Vector2& pos, const DirectX::SimpleMath::Vector2& size,
	const TextureResource* pTexture, int textureIndex)
	: ScaleUpUI(eventID, layer, pos, size, pTexture, textureIndex)
	, m_onMouseEnterSoundId(SoundID::Cursor)
	, m_onClickSoundId(SoundID::Decision) {
}


/// <summary>
/// マウス侵入時処理
/// </summary>
void SoundScaleUpUI::OnMouseEnter() {
	// 効果音を鳴らす
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onMouseEnterSoundId);
}

/// <summary>
/// クリック時処理
/// </summary>
void SoundScaleUpUI::OnClick() {
	ScaleUpUI::OnClick();
	// 効果音を鳴らす処理を追加する
	ServiceLocater<AudioManager>::Get()->PlaySound(m_onClickSoundId);
}
