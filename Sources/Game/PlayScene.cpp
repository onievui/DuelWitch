#include "PlayScene.h"
#include <Framework\DirectX11.h>
#include "ISceneRequest.h"


/// <summary>
///	プレイシーンを初期化する
/// </summary>
/// <param name="sceneRequest"></param>
void PlayScene::Initialize(ISceneRequest* sceneRequest) {
	m_sceneRequest = sceneRequest;
	// スプライトフォントを生成する
	m_font = std::make_unique<DirectX::SpriteFont>(DirectX11::Get().GetDevice().Get(), L"myfile.spritefont");
}

/// <summary>
/// プレイシーンを更新する
/// </summary>
/// <param name="timer"></param>
void PlayScene::Update(const DX::StepTimer& timer) {
	timer;
	
}

/// <summary>
/// プレイシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void PlayScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	
	spriteBatch->End();
}

/// <summary>
/// プレイシーンを終了する
/// </summary>
void PlayScene::Finalize() {

}

