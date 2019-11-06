#pragma once
#ifndef CHARA_SELECT_MARKER_DEFINED
#define CHARA_SELECT_MARKER_DEFINED


#include <Utils\UIObserver.h>
#include <Utils\UISubject.h>


/// <summary>
/// キャラセレクト用マーカー
/// </summary>
class CharaSelectMarker : public UIObserver, public UISubject {
public:
	// コンストラクタ
	CharaSelectMarker(int layer,
		const DirectX::SimpleMath::Vector2& pos,
		const TextureResource* pTexture,
		int textureIndex = 0);

public:
	// オブザーバを更新する
	void Update(ISubject* subject) override;

public:
	// マーカーを更新する
	void Update(const DX::StepTimer& timer) override { timer; }

public:
	// オブザーバの機能を有効にするかを設定する
	void SetEnableObserver(bool enable) { m_enableObserver = enable; }

private:
	// オブザーバの機能が有効かどうかのフラグ
	bool m_enableObserver;

};


#endif // !CHARA_SELECT_MARKER_DEFINED
