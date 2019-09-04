#pragma once
#ifndef TARGET_CAMERA_DEFINED
#define TARGET_CAMERA_DEFINED


#include "Camera.h"


class IObject;


/// <summary>
/// 追従カメラクラス
/// </summary>
class TargetCamera : public Camera {
public:
	// コンストラクタ
	TargetCamera(int width, int height, IObject* targetObject);
	TargetCamera(IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = DirectX::XM_PIDIV4,
		float aspectRatio = 800.0f / 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

public:
	// ターゲットカメラを更新する
	void Update();
	// ターゲットカメラの位置を取得する
	const DirectX::SimpleMath::Vector3& GetEyePosition() const override { return m_pos; }
	// 追従するオブジェクトを設定する
	void SetTargetObject(IObject* pTargetObject) { m_pTargetObject = pTargetObject; }
	// 追従するオブジェクトが存在するかどうか取得する
	bool HasTargetObject();

private:
	// 追従するオブジェクト
	IObject*					 m_pTargetObject;
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

