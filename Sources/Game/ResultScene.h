#pragma once
#ifndef RESULT_SCENE_DEFINED
#define RESULT_SCENE_DEFINED


#include "IScene.h"
#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// リザルトシーンクラス
/// </summary>
class ResultScene : public IScene {
public:
	// コンストラクタ
	ResultScene();
	// デストラクタ
	~ResultScene();
public:
	// リザルトシーンを初期化する
	void Initialize(ISceneRequest* pSceneRequest) override;
	// リザルトシーンを更新する
	void Update(const DX::StepTimer& timer) override;
	// リザルトシーンを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) override;
	// リザルトシーンを終了する
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


#endif // !RESULT_SCENE_DEFINED


