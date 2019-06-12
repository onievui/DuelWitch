#pragma once
#ifndef TARGET_CAMERA_DEFINED
#define TARGET_CAMERA_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>

class ModelObject;

/// <summary>
/// 追従カメラクラス
/// </summary>
class TargetCamera {
public:
	// コンストラクタ
	TargetCamera(int width, int height, ModelObject* targetObject);
	TargetCamera(ModelObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = 3.14f / 4.0f,
		float aspectRatio = 800.0f / 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	// ターゲットカメラを更新する
	void Update();
	// ターゲットカメラのビュー行列を取得する
	DirectX::SimpleMath::Matrix GetViewMatrix() const;
	// ターゲットカメラの射影行列を取得する
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const;
	// ターゲットカメラの位置を取得する
	DirectX::SimpleMath::Vector3 GetEyePosition() const;
	// ターゲットカメラの追従速度を設定する


private:
	// ビュー行列
	DirectX::SimpleMath::Matrix  m_view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix  m_proj;
	// 追従するオブジェクト
	ModelObject*                 m_targetObject;
	DirectX::SimpleMath::Matrix  m_targetMatrix;
	// ターゲットカメラの位置
	DirectX::SimpleMath::Vector3 m_pos;
	// 視点
	DirectX::SimpleMath::Vector3 m_eye;
	// 注視点
	DirectX::SimpleMath::Vector3 m_target;
	// 追従速度
	float                        m_lerpSpeed;
};

#endif	// TARGET_CAMERA_DEFINED

