#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED


#include <map>
#include <Framework\StepTimer.h>
#include <Game\Object\Transform.h>
#include "FieldData.h"


class Player;
class FieldShieldEffectEmitter;


/// <summary>
/// フィールドクラス
/// </summary>
class Field {
public:
	// コンストラクタ
	Field();
	// デストラクタ
	~Field();

public:
	// フィールドを更新する
	void Update(const DX::StepTimer& timer);
	// フィールドを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// フィールドの半径を取得する
	float GetRadius() const { return m_radius; }
	// プレイヤーとフィールドの当たり判定を行う
	void CollisionCheckPlayer(Player& player);

private:
	// フィールド衝突エフェクトを生成する
	void CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos);

private:
	// フィールドのサイズ変更用タイマー
	float                                                                    m_time;
	// 姿勢
	Transform                                                                m_transform;
	// フィールド外壁
	std::unique_ptr<DirectX::GeometricPrimitive>                             m_wall;
	// フィールド半径
	float                                                                    m_radius;
	// フィールド情報
	FieldData                                                                m_fieldData;

	// フィールド衝突エフェクト
	FieldShieldEffectEmitter*                                                m_pEffect;
	// エフェクトを発生させるかどうかの判定用タイマー
	std::map<const Player*, float>                                           m_effectTimer;
};




#endif // !FIELD_DEFINED
