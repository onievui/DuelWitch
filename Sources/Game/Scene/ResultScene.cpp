#include "ResultScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"


/// <summary>
/// コンストラクタ
/// </summary>
ResultScene::ResultScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
ResultScene::~ResultScene() {
}

/// <summary>
///	リザルトシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void ResultScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

}

/// <summary>
/// リザルトシーンを更新する
/// </summary>
/// <param name="timer"></param>
void ResultScene::Update(const DX::StepTimer& timer) {
	timer;

	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		m_pSceneRequest->RequestScene(SceneID::CharaSelect);
	}
}

/// <summary>
/// リザルトシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void ResultScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin();

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"ResultScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// リザルトシーンを終了する
/// </summary>
void ResultScene::Finalize() {

}

