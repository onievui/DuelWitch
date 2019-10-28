#pragma once
#ifndef CHARA_ICON_DEFINED
#define CHARA_ICON_DEFINED


#include <Utils\UISubject.h>


/// <summary>
/// キャラアイコン
/// </summary>
class CharaIcon : public UISubject {
public:
	// コンストラクタ
	CharaIcon(int layer,
		const DirectX::SimpleMath::Vector2& pos,
		const TextureResource* pTexture,
		int textureIndex = 0);

public:
	// マウスオーバー時処理
	void OnMouseOver() override;
};


#endif // !CHARA_ICON_DEFINED
