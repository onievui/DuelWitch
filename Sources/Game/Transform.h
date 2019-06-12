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
		const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// 座標を取得する
	const DirectX::SimpleMath::Vector3& GetPosition() const;
	// 座標を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& position);
	// 回転を取得する
	const DirectX::SimpleMath::Vector3& GetRotation() const;
	// 回転を設定する
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation);
	// スケールを取得する
	const DirectX::SimpleMath::Vector3& GetScale() const;
	// スケールを設定する
	void SetScale(const DirectX::SimpleMath::Vector3& scale);

	// ワールド行列を取得する
	DirectX::SimpleMath::Matrix GetMatrix() const;

private:
	// 座標
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotation;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;

};



#endif // !TRANSFORM_DEFINED
