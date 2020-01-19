#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED


#include <Framework\StepTimer.h>
#include <Utils\MathUtils.h>
#include <Utils\LoadDataHolder.h>
#include <Game\Object\IObject.h>
#include <Game\Object\Transform.h>
#include <Game\Collision\SphereCollider.h>


enum class ElementID;


/// <summary>
/// エレメントクラス
/// </summary>
class Element : public IObject {
public:
	// 当たり判定の半径
	static constexpr float ELEMENT_RADIUS = 0.75f;
	// 回転速度
	static constexpr float ELEMENT_ROTATE_SPEED = Math::HarfPI;

public:
	// コンストラクタ
	Element(DirectX::BasicEffect* pBasicEffect, ID3D11InputLayout* pInputLayout);
	// デストラクタ
	~Element();

public:
	// エレメントを更新する
	void Update(const DX::StepTimer& timer) override;
	// エレメントを生成する
	void Create(ElementID id, const DirectX::SimpleMath::Vector3& pos);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// エレメントの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override { return m_world; }
	// エレメントの当たり判定を取得する
	const Collider* GetCollider() const override                  { return &m_sphereCollider; }
	// エレメントの座標を取得する
	const DirectX::SimpleMath::Vector3& GetPos() const            { return m_transform.GetLocalPosition(); }
	// エレメントのIDを取得する
	ElementID GetID() const                                       { return m_id; }
	// エレメントを使用しているかどうか取得する
	bool IsUsed() const                                           { return m_isUsed; }
	// エレメントを使用するかどうか設定する
	void SetUsed(bool isUsed)                                     { m_isUsed = isUsed; }
	// フィールド端との距離を記憶する
	void SetBetweenFieldRadius(const DirectX::SimpleMath::Vector3& center, float radius);
	// フィールドとの距離を保つ
	void FitField(const DirectX::SimpleMath::Vector3& center, float radius);

private:
	// エレメントID
	ElementID                                    m_id;
	// 姿勢
	Transform                                    m_transform;
	// 球当たり判定
	SphereCollider                               m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
	// ベーシックエフェクトへのポインタ
	DirectX::BasicEffect*                        m_pBasicEffect;
	// 入力レイアウトへのポインタ
	ID3D11InputLayout*                           m_pInputLayout;
	// 使用しているかどうか
	bool                                         m_isUsed;
	// 生成されてからの時間
	float                                        m_time;
	// 生成されたときの、フィールド端までの距離
	float                                        m_betweenFieldEnd;

};


#endif // !ELEMENT_DEFINED


