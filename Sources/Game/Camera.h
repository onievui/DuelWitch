#pragma once
#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED


/// <summary>
/// 透視投影に関する情報クラス
/// </summary>
class PerspectiveFovInfo {
public:
	// コンストラクタ
	PerspectiveFovInfo(float fov, float aspectRatio, float nearPlane, float farPlane) 
		: m_fov(fov)
		, m_aspectRatio(aspectRatio)
		, m_nearPlane(nearPlane)
		, m_farPlane(farPlane) {
	}

public:
	// 射影行列を生成する
	DirectX::SimpleMath::Matrix CreateMatrix() {
		return DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
	}

public:
	// 画角
	float m_fov;
	// アスペクト比
	float m_aspectRatio;
	// 前面クリップ面
	float m_nearPlane;
	// 後方クリップ面
	float m_farPlane;
};

/// <summary>
/// カメラクラス
/// </summary>
class Camera {
public:
	// カメラのビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }
	// カメラの射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_proj; }
	// カメラの位置を取得する
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }
	// カメラの位置を設定する
	virtual void SetEyePosition(DirectX::SimpleMath::Vector3 eye) { m_eye = eye; }
	// カメラの注視点を取得する
	virtual const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }
	// カメラの注視点を設定する
	virtual void SetTargetPosition(DirectX::SimpleMath::Vector3 target) { m_target = target; }

	// 画角を設定する
	virtual void SetFov(float fov) { fov; }
	// 画角を取得する
	virtual float GetFov() const { return 0.0f; }


public:
	// 視線ベクトルを取得する
	DirectX::SimpleMath::Vector3 GetEyeVector() const;

	// スクリーン座標からレイに変換する
	DirectX::SimpleMath::Ray ScreenPointToRay(const DirectX::SimpleMath::Vector3& point) const;
	// ビューポート座標からレイに変換する
	DirectX::SimpleMath::Ray ViewportPointToRay(const DirectX::SimpleMath::Vector3& point) const;

	// ワールド座標からスクリーン座標に変換する
	DirectX::SimpleMath::Vector3 WorldToScreenPoint(const DirectX::SimpleMath::Vector3& point) const;
	// ワールド座標からビューポート座標に変換する
	DirectX::SimpleMath::Vector3 WorldToViewportPoint(const DirectX::SimpleMath::Vector3& point) const;
	// ビューポート座標からワールド座標に変換する
	DirectX::SimpleMath::Vector3 ViewportToWorldPoint(const DirectX::SimpleMath::Vector3& point) const;
	// スクリーン座標からワールド座標に変換する
	DirectX::SimpleMath::Vector3 ScreenToWorldPoint(const DirectX::SimpleMath::Vector3& point) const;
	// スクリーン座標からビューポート座標に変換する
	DirectX::SimpleMath::Vector3 ScreenToViewportPoint(const DirectX::SimpleMath::Vector3& point) const;
	// ビューポート座標からスクリーン座標に変換する
	DirectX::SimpleMath::Vector3 ViewportToScreenPoint(const DirectX::SimpleMath::Vector3& point) const;

protected:
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_eye;
	// ビュー行列
	DirectX::SimpleMath::Matrix  m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix  m_proj;
	// ビューポート行列
	DirectX::SimpleMath::Matrix  m_viewport;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
};


#endif // !CAMERA_DEFINED
