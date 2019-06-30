#pragma once
#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED


/// <summary>
/// カメラクラス
/// </summary>
class Camera {
public:
	// カメラのビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const {
		return m_view;
	}
	// カメラの射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const {
		return m_proj;
	}
	// カメラの位置を取得する
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const {
		return m_eye;
	}
	// カメラの位置を設定する
	virtual void SetEyePosition(DirectX::SimpleMath::Vector3 eye) {
		m_eye = eye;
	}
	// カメラの注視点を設定する
	virtual void SetTargetPosition(DirectX::SimpleMath::Vector3 target) {
		m_target = target;
	}

public:
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
