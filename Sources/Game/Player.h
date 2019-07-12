#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


class MagicManager;
class Camera;
class Command;


/// <summary>
/// プレイヤーID
/// </summary>
enum class PlayerID {
	Player1,
	Player2
};

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IObject {
	friend class Command;

public:
	// 進行方向
	enum class MoveDirection {
		Forward,
		Backward
	};

public:
	Player(MagicManager* magicManager, PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction);
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
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const;
	// 敵プレイヤーを設定する
	void SetOtherPlayer(Player* otherPlayer);
	// カメラを設定する
	void SetCamera(Camera* camera);
	// プレイヤー同士の衝突処理を行う
	void CollisionPlayer(const Player& player);

private:

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// モデル
	std::unique_ptr<DirectX::Model>        m_model;
	// プレイヤーID
	PlayerID                               m_id;
	// 進行方向
	MoveDirection                          m_direction;
	// 姿勢
	Transform                              m_transform;
	// 球当たり判定
	SphereCollider                         m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix            m_world;
	// 移動コマンド
	std::unique_ptr<Command>               m_moveCommand;
	// 詠唱コマンド
	std::unique_ptr<Command>               m_castCommand;
	// 敵プレイヤー
	Player*                                m_otherPlayer;
	// 魔法マネージャ
	MagicManager*                          m_pMagicManager;
	// カメラ
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


