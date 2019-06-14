#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"


/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IObject {
public:
	Player();
	~Player();

	// プレイヤーを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイヤーを開放する
	void Lost() override;
	// プレイヤーを生成する
	void Create(const std::wstring& fileName, const std::wstring& directory = L"");
	// プレイヤーを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// プレイヤーの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;

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


#endif // !PLAYER_DEFINED


