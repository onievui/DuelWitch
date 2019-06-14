#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED

#include <Framework\StepTimer.h>
#include "IObject.h"
#include "Transform.h"

/// <summary>
/// エレメントクラス
/// </summary>
class Element : public IObject{
public:
	Element();
	~Element();

public:
	// エレメントを更新する
	void Update(const DX::StepTimer& timer) override;
	// エレメントを開放する
	void Lost() override;
	// エレメントを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override;

public:
	// エレメントの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const;

private:
	static constexpr float ELEMENT_RADIUS = 1.5f;

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// 姿勢
	Transform                                    m_transform;
	// ワールド行列
	DirectX::SimpleMath::Matrix                  m_world;
};


#endif // !ELEMENT_DEFINED


