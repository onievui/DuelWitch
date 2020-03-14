#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include <list>
#include <Game\Object\IObject.h>
#include <Game\Object\Transform.h>
#include <Game\Collision\SphereCollider.h>
#include "PlayerStatus.h"


class MagicManager;
class IMagic;
class Camera;
class Command;
class RenderCommand;
enum class ElementID;
enum class PlayerID;


/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public IObject {
	// プレイヤーのプライベートなメンバにアクセスできるようにする
	friend class Command;

public:
	// エレメント所持上限
	static constexpr int HAVE_ELEMENT_MAX = 10;

public:
	// コンストラクタ
	Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos);
	// デストラクタ
	~Player();

public:
	// プレイヤーを初期化する
	void Initialize(MagicManager* pMagicManager, Camera* pCamera, std::vector<std::unique_ptr<Player>>& pOtherPlayers);
	// プレイヤーを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイヤーを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const;

public:
	// プレイヤーの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// プレイヤーの当たり判定を取得する
	const Collider* GetCollider() const override;
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const;
	// 他のプレイヤーを設定する
	void SetOtherPlayers(std::vector<std::unique_ptr<Player>>& pOtherPlayers);
	// プレイヤーのHPが0以下か取得する
	bool IsDead() const;
	// プレイヤーがブースト移動中か取得する
	bool IsBoosting() const;
	// 魔法のダメージ倍率を取得する
	float GetMagicPowerRate(ElementID elementId) const;

public:
	// エレメントの取得処理を行う
	void GetElement(ElementID elementId);
	// プレイヤー同士の衝突処理を行う
	void HitPlayer(const Player& player);
	// 魔法との衝突処理を行う
	void HitMagic(const IMagic* magic);
	// フィールドとの衝突処理を行う
	void HitField(const DirectX::SimpleMath::Vector3& hitPos);

private:
	// ステータスを初期化する
	void InitializeStatus();
	// ステータスを更新する
	void UpdateStatus(const DX::StepTimer& timer);

private:
	// プレイヤーID
	PlayerID                               m_id;
	// ステータス
	PlayerStatus                           m_status;
	// 所持エレメント
	std::list<ElementID>                   m_haveElements;
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
	// 描画コマンド
	std::unique_ptr<RenderCommand>         m_renderCommand;

	// 敵プレイヤー
	std::vector<Player*>                   m_pOtherPlayers;
	// 魔法マネージャ
	MagicManager*                          m_pMagicManager;
	// カメラ
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


