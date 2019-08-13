#pragma once
#ifndef TRANSFORM_DEFINED
#define TRANSFORM_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>


/// <summary>
/// 姿勢クラス
/// </summary>
class Transform {
public:
	// コンストラクタ
	Transform(const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Quaternion& rotation = DirectX::SimpleMath::Quaternion::Identity,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);
	// コンストラクタ
	Transform(const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& euler,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// 座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition() const           { return m_position; }
	// 座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& position)    { m_position = position; }
	// 回転を取得する
	const DirectX::SimpleMath::Quaternion& GetRotation() const        { return m_rotation; }
	// 回転を設定する
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) { m_rotation = rotation; }
	// スケールを取得する
	const DirectX::SimpleMath::Vector3& GetScale() const              { return m_scale; }
	// スケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale)          { m_scale = scale; }

	// ワールド行列を取得する
	DirectX::SimpleMath::Matrix GetMatrix() const;

private:
	// 座標
	DirectX::SimpleMath::Vector3    m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// スケール
	DirectX::SimpleMath::Vector3    m_scale;

};



#endif // !TRANSFORM_DEFINED
