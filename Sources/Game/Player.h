#pragma once
#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

#include <Framework\StepTimer.h>
#include <list>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


class MagicManager;
class IMagic;
class Camera;
class Command;
class RenderCommand;
enum class ElementID;
enum class PlayerID;
enum class MoveCommandState;


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
	// コンストラクタ
	Player(PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction);
	// デストラクタ
	~Player();

public:
	// プレイヤーを初期化する
	void Initialize(MagicManager* pMagicManager, Camera* pCamera, Player* pOtherPlayer);
	// プレイヤーを更新する
	void Update(const DX::StepTimer& timer) override;
	// プレイヤーを開放する
	void Lost() override;
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
	// 魔法のダメージ倍率を取得する
	float GetMagicPowerRate(ElementID elementId) const;
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

public:
	/// <summary>
	/// プレイヤーステータス
	/// </summary>
	struct Status {
		// 外部データ
		// 最大HP
		float                             maxHp;
		// HP
		float                             hp;
		// 前フレームのHP
		float                             preHp;
		// 最大SP
		float                             maxSp;
		// SP
		float                             sp;
		// 前フレームのSP
		float                             preSp;
		// SP回復速度
		float                             spRecoverySpeed;
		// 通常魔法の消費SP
		float                             normalMagicSpCost;
		// ブースト速度倍率
		float                             boostSpeedRate;
		// ブースト消費SP
		float                             boostSpCost;
		// 1段階目のチャージタイム
		float                             firstChargeTime;
		// 2段階目のチャージタイム
		float                             secoundChargeTime;
		// 炎魔法のダメージ倍率
		float                             fireMagicPowerRate;
		// 氷魔法のダメージ倍率
		float                             freezeMagicPowerRate;
		// 雷魔法のダメージ倍率
		float                             thunderMagicPowerRate;

		// 内部データ
		// ダメージ後無敵時間タイマー
		float                             damageTimer;
		// SP消費直後タイマー
		float                             spDecreaseTimer;
		// ブースト中かどうか
		bool                              isBoosting;
		// チャージ段階
		int                               chargeLevel;
		// 移動コマンドの状態
		MoveCommandState                  moveCommandState;
	};

private:
	// プレイヤーID
	PlayerID                               m_id;
	// ステータス
	Status                                 m_status;
	// 進行方向
	MoveDirection                          m_direction;
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
	Player*                                m_pOtherPlayer;
	// 魔法マネージャ
	MagicManager*                          m_pMagicManager;
	// カメラ
	Camera*                                m_pCamera;
};


#endif // !PLAYER_DEFINED


