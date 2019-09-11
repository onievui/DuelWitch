#include "TitleScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {
}

/// <summary>
///	タイトルシーンを初期化する
/// </summary>
/// <param name="pSceneRequest"></param>
void TitleScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	ResourceLoader::Load(ResourceLoaderID::TitleScene);
	m_time = 0.0f;
}

/// <summary>
/// タイトルシーンを更新する
/// </summary>
/// <param name="timer"></param>
void TitleScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_time += elapsed_time;

	// スペースキーを押した場合の処理
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
		// タイトルが表示されていなければ、タイマーを進めて表示させる
		if (m_time < 2.0f) {
			m_time = 2.0f;
		}
		// キャラセレクトシーンに進む
		else {
			m_pSceneRequest->RequestScene("CharaSelect");
		}
	}
}

/// <summary>
/// タイトルシーンを描画する
/// </summary>
/// <param name="spriteBatch"></param>
void TitleScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Title);
	// 時間経過でフェードインを行う
	float alpha;
	if (m_time <= 2.0f) {
		alpha = std::max(m_time - 1.0f, 0.0f);
	}
	else {
		alpha = 1.0f;
	}
	// タイトルを描画する
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		texture->GetCenter(), DirectX::SimpleMath::Vector2(1.0f, 1.0f)*1.25f);

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"TitleScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// タイトルシーンを終了する
/// </summary>
void TitleScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::TitleScene);
}

