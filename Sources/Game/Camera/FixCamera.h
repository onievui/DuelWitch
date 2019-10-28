#pragma once
#ifndef FIX_CAMERA_DEFINED
#define FIX_CAMERA_DEFINED


/// <summary>
/// 追従カメラクラス
/// </summary>
class FixCamera {
public:
	// コンストラクタ
	FixCamera(int width, int height);
	FixCamera(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = DirectX::XM_PIDIV4,
		float aspectRatio = 800.0f/600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	// 固定カメラを更新する
	void Update();
	// 固定カメラのビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const;
	// 固定カメラの射影行列を取得する
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const;
	// 固定カメラの位置を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const;
	// 固定カメラの位置を設定する
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye);
	// 固定カメラの注視点を設定する
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target);

private:
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix m_proj;
	// ターゲットカメラの位置
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

};

#endif	// FIX_CAMERA_DEFINED

