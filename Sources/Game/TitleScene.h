#pragma once
#ifndef TITLE_SCENE_DEFINED
#define TITLE_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene : public IScene {
public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();
public:
	// タイトルシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// タイトルシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// タイトルシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// タイトルシーンを終了する
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


#endif // !TITLE_SCENE_DEFINED


