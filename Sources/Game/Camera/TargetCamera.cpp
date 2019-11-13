#include "TargetCamera.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Game\Object\IObject.h>


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="targetObject">追従するオブジェクト</param>
/// <param name="eye">視点の相対座標</param>
/// <param name="target">注視点の相対座標</param>
/// <param name="up">カメラの上方向</param>
/// <param name="perspectiveFovInfo">透視投影に関する情報</param>
TargetCamera::TargetCamera(const IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
	DirectX::SimpleMath::Vector3 up, PerspectiveFovInfo perspectiveFovInfo)
	: m_pTargetObject(targetObject)
	, m_relativeEye(eye)
	, m_relativeTarget(target)
	, m_lerpSpeed(0.5f)
	, m_perspectiveFovInfo(perspectiveFovInfo) {
	m_eye = eye;
	m_target = target;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
	m_proj = perspectiveFovInfo.CreateMatrix();
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	m_viewport = DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(0.5f, -0.5f, 1.0f)) *
		DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f)) *
		DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(static_cast<float>(directX->GetWidth()),
			static_cast<float>(directX->GetHeight()), 1.0f));
	if (m_pTargetObject) {
		m_targetMatrix = m_pTargetObject->GetMatrix();
	}
}

/// <summary>
/// ターゲットカメラを更新する
/// </summary>
void TargetCamera::Update() {
	m_targetMatrix = DirectX::SimpleMath::Matrix::Lerp(m_targetMatrix, m_pTargetObject->GetMatrix(), m_lerpSpeed);
	m_eye = DirectX::SimpleMath::Vector3::Transform(m_relativeEye, m_matrix*m_targetMatrix);
	m_target = DirectX::SimpleMath::Vector3::Transform(m_relativeTarget, m_matrix*m_targetMatrix);
	//DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY, m_targetMatrix);
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
}

/// <summary>
/// 追従するオブジェクトが存在するかどうか取得する
/// </summary>
/// <returns>
/// true : 存在する
/// false : 存在しない
/// </returns>
bool TargetCamera::HasTargetObject() {
	return m_pTargetObject != nullptr;
}

/// <summary>
/// 画角を設定する
/// </summary>
/// <param name="fov">画角</param>
void TargetCamera::SetFov(float fov) {
	m_perspectiveFovInfo.m_fov = fov;
	m_proj = m_perspectiveFovInfo.CreateMatrix();
}

