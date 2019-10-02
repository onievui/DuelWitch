#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED


#include <map>
#include <Framework\StepTimer.h>
#include "Transform.h"


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
	// プレイヤーとフィールドの当たり判定を行う
	void CollisionCheckPlayer(Player& player);

private:
	// フィールド衝突エフェクトを生成する
	void CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos);
	// 目印となる画像を描画する
	void DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                                   m_states;
	// スカイドーム
	std::unique_ptr<DirectX::Model>                                          m_skydome;
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect>                                m_batchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                m_inputLayout;

	// Uターン画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                         m_turnTexture;

	// 姿勢
	Transform                                                                m_transform;
	// フィールド外壁
	std::unique_ptr<DirectX::GeometricPrimitive>                             m_wall;
	// フィールド半径
	float                                                                    m_radius;

	// フィールド衝突エフェクト
	FieldShieldEffectEmitter*                                                m_pEffect;
	// エフェクトを発生させるかどうかの判定用タイマー
	std::map<const Player*, float>                                           m_effectTimer;
};




#endif // !FIELD_DEFINED
