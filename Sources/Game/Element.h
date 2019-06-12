#pragma once
#ifndef ELEMENT_DEFINED
#define ELEMENT_DEFINED

#include <Framework\StepTimer.h>

#include "Transform.h"

/// <summary>
/// エレメントクラス
/// </summary>
class Element {
public:
	Element();
	~Element();

	// エレメントを更新する
	void Update(const DX::StepTimer& timer);
	// エレメントを開放する
	void Lost();
	// エレメントを生成する
	void Create(const DirectX::SimpleMath::Vector3& pos);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

private:
	static constexpr float s_radius = 1.5f;

private:
	// オブジェクト
	std::unique_ptr<DirectX::GeometricPrimitive> m_object;
	// 姿勢
	Transform                                    m_transform;
};


#endif // !ELEMENT_DEFINED


