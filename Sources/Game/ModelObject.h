#pragma once
#ifndef MODEL_OBJECT_DEFINED
#define MODEL_OBJECT_DEFINED

#include <Framework\StepTimer.h>
//#include "DirectX11.h"
//#include <string>
//#include "StepTimer.h"
//#include <SimpleMath.h>
//#include <Effects.h>
//#include <CommonStates.h>
//#include <Model.h> 

#include "Transform.h"

/// <summary>
/// モデルオブジェクトクラス
/// </summary>
class ModelObject {
public:
	ModelObject();
	~ModelObject();

	// モデルオブジェクトを更新する
	void Update(const DX::StepTimer& timer);
	// モデルオブジェクトを開放する
	void Lost();
	// モデルオブジェクトを生成する
	void Create(const std::wstring& fileName, const std::wstring& directory = L"");
	// モデルオブジェクトを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

public:
	// モデルオブジェクトの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const;

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// モデル
	std::unique_ptr<DirectX::Model>        m_model;
	// 姿勢
	Transform                              m_transform;
	// ワールド行列
	DirectX::SimpleMath::Matrix            m_world;
};


#endif // !MODEL_OBJECT_DEFINED


