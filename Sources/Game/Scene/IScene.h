#pragma once
#ifndef ISCENE_DEFINED
#define ISCENE_DEFINED

#include <Framework\StepTimer.h>


class ISceneRequest;


/// <summary>
/// シーンインタフェースクラス
/// </summary>
class IScene {
public:
	// シーンを初期化する
	virtual void Initialize(ISceneRequest* pSceneRequest) = 0;
	// シーンを更新する
	virtual void Update(const DX::StepTimer& timer) = 0;
	// シーンを描画する
	virtual void Render(DirectX::SpriteBatch* spriteBatch) = 0;
	// シーンを終了する
	virtual void Finalize() = 0;

};

#endif // !ISCENE_DEFINED


