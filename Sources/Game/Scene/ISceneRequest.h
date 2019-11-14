#pragma once
#ifndef ISECENE_REQUEST_DEFINED
#define ISECENE_REQUEST_DEFINED


#include "SceneID.h"


/// <summary>
/// リクエストシーンタイプ
/// </summary>
enum class RequestSceneType {
	StackScene,		//今までのシーンの上に追加する
	ClearStack		//今までのシーンを消して追加する
};


/// <summary>
/// シーンリクエストインタフェースクラス
/// </summary>
class ISceneRequest {
public:
	// シーンの変更を要求する
	virtual void RequestScene(SceneID sceneId, RequestSceneType type = RequestSceneType::ClearStack) = 0;
	// シーンをポップする
	virtual void PopScene(int num = 1) = 0;
};

#endif // !ISECENE_REQUEST_DEFINED


