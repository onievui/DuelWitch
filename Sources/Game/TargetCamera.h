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
	TargetCamera(IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up, PerspectiveFovInfo perspectiveFovInfo);

public:
	// ターゲットカメラを更新する
	void Update();
	// 追従するオブジェクトを設定する
	void SetTargetObject(IObject* pTargetObject) { m_pTargetObject = pTargetObject; }
	// 追従するオブジェクトが存在するかどうか取得する
	bool HasTargetObject();
	// 画角を設定する
	void SetFov(float fov);
	// 画角を取得する
	float GetFov() const { return m_perspectiveFovInfo.m_fov; }

private:
	// 追従するオブジェクト
	IObject*					 m_pTargetObject;
	DirectX::SimpleMath::Matrix  m_targetMatrix;
	// 視点の相対位置
	DirectX::SimpleMath::Vector3 m_relativeEye;
	// 注視点の相対位置
	DirectX::SimpleMath::Vector3 m_relativeTarget;
	// 追従速度
	float                        m_lerpSpeed;
	// 透視投影に関する情報
	PerspectiveFovInfo           m_perspectiveFovInfo;
};

#endif	// TARGET_CAMERA_DEFINED

