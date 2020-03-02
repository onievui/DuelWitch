#include "SceneManager.h"
#include <Utils\ErrorMessage.h>
#include <Utils\ServiceLocater.h>
#include "LogoScene.h"
#include "TitleScene.h"
#include "CharaSelectScene.h"
#include "CharaManualScene.h"
#include "PlayScene.h"
#include "PauseScene.h"
#include "WinResultScene.h"
#include "LoseResultScene.h"



/// <summary>
/// シーンの変更を要求する
/// </summary>
/// <param name="sceneId">シーンID</param>
/// <param name="type">リクエストシーンタイプ</param>
void SceneManager::RequestScene(SceneID sceneId, RequestSceneType type) {
	m_requestQueue.push(std::make_pair(sceneId, type));
}

/// <summary>
/// シーンをポップする
/// </summary>
/// <param name="num">ポップするシーンの数</param>
void SceneManager::PopScene(int num) {
	while (!m_activeScene.empty() && num > 0) {
		m_activeScene.back()->Finalize();
		m_activeScene.pop_back();
		--num;
	}
	//// キーボードとマウスをリセットする
	//ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Reset();
	//ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get()->Reset();

}

/// <summary>
/// シーンマネージャを初期化する
/// </summary>
void SceneManager::Initialize() {
	m_sceneCreateFunc.clear();
	m_sceneCreateFunc.emplace(SceneID::Logo,        MakeSceneCreateFunc<LogoScene>());
	m_sceneCreateFunc.emplace(SceneID::Title,       MakeSceneCreateFunc<TitleScene>());
	m_sceneCreateFunc.emplace(SceneID::CharaSelect, MakeSceneCreateFunc<CharaSelectScene>());
	m_sceneCreateFunc.emplace(SceneID::CharaManual, MakeSceneCreateFunc<CharaManualScene>());
	m_sceneCreateFunc.emplace(SceneID::Play,        MakeSceneCreateFunc<PlayScene>());
	m_sceneCreateFunc.emplace(SceneID::Pause,       MakeSceneCreateFunc<PauseScene>());
	m_sceneCreateFunc.emplace(SceneID::WinResult,   MakeSceneCreateFunc<WinResultScene>());
	m_sceneCreateFunc.emplace(SceneID::LoseResult,  MakeSceneCreateFunc<LoseResultScene>());
	std::queue<std::pair<SceneID, RequestSceneType>>().swap(m_requestQueue);
}

/// <summary>
/// シーンマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void SceneManager::Update(const DX::StepTimer& timer) {
	// シーンの変更要求があれば変更する
	while (!m_requestQueue.empty()) {
		ChangeScene();
	}

	// 一番新しいシーンを更新する
	m_activeScene.back()->Update(timer);
}

/// <summary>
/// シーンを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void SceneManager::Render(DirectX::SpriteBatch* spriteBatch) {
	// 有効なシーンを古いものから描画する
	for (std::list<std::unique_ptr<IScene>>::iterator itr = m_activeScene.begin(); itr != m_activeScene.end(); ++itr) {
		(*itr)->Render(spriteBatch);
	}
}

/// <summary>
/// シーンを変更する
/// </summary>
void SceneManager::ChangeScene() {
	std::pair<SceneID, RequestSceneType>& requestData = m_requestQueue.front();
	if (!m_sceneCreateFunc.count(requestData.first)) {
		ErrorMessage(L"不正なシーン名を読み込もうとしました");
		m_requestQueue.pop();
		return;
	}
	//// キーボードとマウスをリセットする
	//ServiceLocater<DirectX::Keyboard::KeyboardStateTracker>::Get()->Reset();
	//ServiceLocater<DirectX::Mouse::ButtonStateTracker>::Get()->Reset();

	// シーンを破棄する場合
	if (requestData.second == RequestSceneType::ClearStack) {
		// 有効なシーンを下から終了する
		while (!m_activeScene.empty()) {
			m_activeScene.back()->Finalize();
			m_activeScene.pop_back();
		}
	}
	m_activeScene.push_back(m_sceneCreateFunc[requestData.first]());
	m_activeScene.back()->Initialize(static_cast<ISceneRequest*>(this));
	m_requestQueue.pop();
}
