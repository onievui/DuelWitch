#include "LogoScene.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include "ISceneRequest.h"
#include "ResourceLoader.h"


/// <summary>
/// コンストラクタ
/// </summary>
LogoScene::LogoScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
LogoScene::~LogoScene() {
}

/// <summary>
///	ロゴシーンを初期化する
/// </summary>
/// <param name="pSceneRequest">リクエストシーンインタフェース</param>
void LogoScene::Initialize(ISceneRequest* pSceneRequest) {
	m_pSceneRequest = pSceneRequest;
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	// コモンステートを生成する
	m_commonStates = std::make_unique<DirectX::CommonStates>(directX->GetDevice().Get());
	// エフェクトファクトリを生成する
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(directX->GetDevice().Get());

	ResourceLoader::Load(ResourceLoaderID::LogoScene);
	m_time = 0.0f;

}

/// <summary>
/// ロゴシーンを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void LogoScene::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// スペースキーを押すか5秒経過でタイトルシーンに移行する
	if (ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->IsKeyPressed(DirectX::Keyboard::Keys::Space) ||
		m_time > 5.0f) {
		m_pSceneRequest->RequestScene("Title");
	}
}

/// <summary>
/// ロゴシーンを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void LogoScene::Render(DirectX::SpriteBatch* spriteBatch) {
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());

	const DirectX11* directX = ServiceLocater<DirectX11>::Get();
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Logo);
	// 時間経過でフェードイン、アウトを行う
	float alpha;
	if (m_time <= 2.0f) {
		alpha = std::max(m_time - 1.0f, 0.0f);
	}
	else if (m_time <= 3.0f) {
		alpha = 1.0f;
	}
	else {
		alpha = std::max(4.0f - m_time, 0.0f);
	}
	spriteBatch->Draw(texture->GetResource().Get(),
		DirectX::SimpleMath::Vector2(directX->GetWidth()*0.5f, directX->GetHeight()*0.5f),
		nullptr, DirectX::SimpleMath::Vector4(1, 1, 1, alpha), 0,
		DirectX::SimpleMath::Vector2(320.0f, 240.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	const DirectX::SpriteFont* font = ServiceLocater<ResourceManager<FontResource>>::Get()->GetResource(FontID::Default)->GetResource().get();
	font->DrawString(spriteBatch, L"LogoScene press Space", DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);

	spriteBatch->End();
}

/// <summary>
/// ロゴシーンを終了する
/// </summary>
void LogoScene::Finalize() {
	ResourceLoader::Release(ResourceLoaderID::LogoScene);
}

