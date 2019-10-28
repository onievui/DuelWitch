#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED


#include <Framework\StepTimer.h>
#include <Utils\LoadDataHolder.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


enum class ElementID;


/// <summary>
/// エレメントクラス
/// </summary>
class Element : public IObject {
public:
	static constexpr float ELEMENT_RADIUS = 0.75f;

public:
	Element();
	~Element();

public:
	// エレメントを更新する
	void Update(const DX::StepTimer& timer) override;
	// エレメントを開放する
	void Lost() override;
	// エレメントを生成する
	void Create(ElementID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// エレメントの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override { return m_world; }
	// エレメントの当たり判定を取得する
	const Collider* GetCollider() const override                  { return &m_sphereCollider; }
	// エレメントの座標を取得する
	const DirectX::SimpleMath::Vector3& GetPos() const            { return m_transform.GetPosition(); }
	// エレメントのIDを取得する
	ElementID GetID() const                                       { return m_id; }
	// エレメントを使用しているかどうか取得する
	bool IsUsed() const                                           { return m_isUsed; }
	// エレメントを使用するかどうか設定する
	void SetUsed(bool isUsed)                                     { m_isUsed = isUsed; }
	// フィールドの範囲内に収める
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
	// 色
	DirectX::SimpleMath::Color                   m_color;
	// 使用しているかどうか
	bool                                         m_isUsed;

};


#endif // !ELEMENT_DEFINED


