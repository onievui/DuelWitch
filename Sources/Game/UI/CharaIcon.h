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
	// UIを更新する
	void Update(const DX::StepTimer& timer) override;

public:
	// マウス侵入時処理
	void OnMouseEnter() override;
	// マウスオーバー時処理
	void OnMouseOver() override;
	// クリック時処理
	void OnClick() override;

public:
	// UIを選択する
	void Select();
	// UIを選択解除する
	void Deselect() { m_isSelected = false; }

private:
	// UIが選択されているかどうか
	bool                  m_isSelected;

};


#endif // !CHARA_ICON_DEFINED
