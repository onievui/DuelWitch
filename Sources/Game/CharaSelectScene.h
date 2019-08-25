#pragma once
#ifndef CHARA_SELECT_SCENE_DEFINED
#define CHARA_SELECT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// キャラセレクトシーンクラス
/// </summary>
class CharaSelectScene : public IScene {
public:
	// コンストラクタ
	CharaSelectScene();
	// デストラクタ
	~CharaSelectScene();
public:
	// キャラセレクトシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// キャラセレクトシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// キャラセレクトシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// キャラセレクトシーンを終了する
	void Finalize() override;

private:
	// リクエストシーンインタフェース
	ISceneRequest*                                           m_pSceneRequest;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont>                     m_font;
	// エフェクトファクトリインタフェース
	std::unique_ptr<DirectX::IEffectFactory>                 m_effectFactory;
	// コモンステート
	std::unique_ptr <DirectX::CommonStates>                  m_commonStates;

};


#endif // !CHARA_SELECT_SCENE_DEFINED


