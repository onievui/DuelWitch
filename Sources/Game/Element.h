#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED


#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"
#include "SphereCollider.h"


/// <summary>
/// エレメントクラス
/// </summary>
class Element : public IObject {
private:
	static constexpr float ELEMENT_RADIUS = 1.5f;

public:
	Element();
	~Element();

public:
	// エレメントを更新する
	void Update(const DX::StepTimer& timer) override;
	// エレメントを開放する
	void Lost() override;
	// エレメントを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector4& color);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// エレメントの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
	// エレメントの当たり判定を取得する
	const SphereCollider* GetCollider() const override;
	// エレメントを使用しているかどうか取得する
	bool IsUsed() const;
	// エレメントを使用するかどうか設定する
	void IsUsed(bool isUsed);

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// 姿勢
	Transform                                    m_transform;
	// 球当たり判定
	SphereCollider                               m_sphereCollider;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
	// 色
	DirectX::SimpleMath::Vector4                 m_color;
	// 使用しているかどうか
	bool                                         m_isUsed;
};


#endif // !ELEMENT_DEFINED


