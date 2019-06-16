#pragma once
#ifndef SCENE_MANAGER_DEFINED
#define SCENE_MANAGER_DEFINED

#include <functional>
#include <unordered_map>
#include <queue>

#include <Framework\StepTimer.h>

#include "IScene.h"
#include "ISceneRequest.h"


/// <summary>
/// シーンマネージャクラス
/// </summary>
class SceneManager : public ISceneRequest {
public:
	// シーンの変更を要求する
	void RequestScene(const std::string& sceneName, RequestSceneType type = RequestSceneType::ClearStack) override;
	// シーンをポップする
	void PopScene(int num = 1) override;

public:
	// シーンマネージャを初期化する
	virtual void Initialize();
	// シーンマネージャを更新する
	virtual void Update(const DX::StepTimer& timer);
	// シーンを描画する
	virtual void Render(DirectX::SpriteBatch* spriteBatch);

public:
	// シーンを変更する
	void ChangeScene();

private:
	// シーン生成関数
	using SceneCreateFunc = std::function<std::unique_ptr<IScene>()>;

	template<class Scene>
	/// <summary>
	/// シーン生成関数を作成する
	/// </summary>
	/// <returns>
	/// シーン生成関数
	/// </returns>
	SceneCreateFunc MakeSceneCreateFunc() {
		return []() {return std::make_unique<Scene>(); };
	}

private:
	// シーン生成関数
	std::unordered_map<std::string, SceneCreateFunc>     m_sceneCreateFunc;
	// 有効なシーンのリスト
	std::list<std::unique_ptr<IScene>>                   m_activeScene;
	// シーン変更要求のキュー
	std::queue<std::pair<std::string, RequestSceneType>> m_requestQueue;

};


#endif // !SCENE_MANAGER_DEFINED