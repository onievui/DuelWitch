#pragma once
#ifndef MAGIC_DEFINED
#define MAGIC_DEFINED


#include "IMagic.h"
#include <Game\Object\Transform.h>


class Collider;


/// <summary>
/// 魔法クラス
/// </summary>
class Magic : public IMagic {
public:
	virtual ~Magic() = default;

protected:
	Magic()
		: m_info()
		, m_transform()
		, m_collider()
		, m_world()
		, m_vel()
		, m_color(DirectX::Colors::White)
		, m_isUsed(false) {

	}

public:
	// 魔法を生成する
	virtual void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
		m_info = magicInfo;
		m_transform.SetPosition(pos);
		m_vel = dir;
	}
	// 魔法を更新する
	virtual void Update(const DX::StepTimer& timer) { timer; }
	// 魔法を開放する
	virtual void Lost() {}	
	// 魔法を描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const { view; proj; }
	// 魔法の終了処理を行う
	virtual void Finalize() override {}

public:
	// オブジェクトの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const { return m_world; }
	// オブジェクトの当たり判定を取得する
	const Collider* GetCollider() const                  { return m_collider.get(); }
	// 魔法IDを取得する
	MagicID GetID() const                                { return m_info.id; }
	// プレイヤーIDを取得する
	PlayerID GetPlayerID() const                         { return m_info.playerId; }
	// ダメージを取得する
	virtual float GetPower() const                       { return m_info.powerRate; }
	// プレイヤーとの衝突処理
	virtual void HitPlayer(const Collider* collider)     { collider; }
	// 魔法との衝突処理
	virtual void HitMagic(const IMagic* other)           { other; }
	// 魔法を使用しているかどうか取得する
	bool IsUsed() const                                  { return m_isUsed; }
	// 魔法を使用するかどうか設定する
	void SetUsed(bool isUsed)                            { m_isUsed = isUsed; }

protected:
	// 魔法に関する情報
	MagicInfo                                    m_info;
	// 姿勢
	Transform                                    m_transform;
	// 球当たり判定
	std::unique_ptr<Collider>                    m_collider;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
	// 速度
	DirectX::SimpleMath::Vector3                 m_vel;
	// 色
	DirectX::SimpleMath::Color                   m_color;
	// 使用しているかどうか
	bool                                         m_isUsed;
	// 生存時間
	float                                        m_lifeTime;

};


#endif // !MAGIC_DEFINED


