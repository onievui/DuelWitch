#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


class MagicManager;


/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IObject {
public:
	Player(MagicManager* magicManager);
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
	// プレイヤーの当たり判定を取得する
	const SphereCollider* GetCollider() const override;

private:
	// 移動を行う
	void Move(const DX::StepTimer& timer);
	// 魔法を唱える
	void CastMagic(const DX::StepTimer& timer);

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// モデル
	std::unique_ptr<DirectX::Model>        m_model;
	// 姿勢
	Transform                              m_transform;
	// 球当たり判定
	SphereCollider                         m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix            m_world;
	// 魔法マネージャ
	MagicManager*                          m_pMagicManager;
};


#endif // !PLAYER_DEFINED


