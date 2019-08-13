#include "TargetCamera.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include "IObject.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="width">画面の横幅</param>
/// <param name="height">画面の縦幅</param>
/// <param name="targetObject">追従するオブジェクト</param>
TargetCamera::TargetCamera(int width, int height, IObject* targetObject)
	: m_targetObject(targetObject)
	, m_pos()
	, m_lerpSpeed(0.05f) {
	m_eye = DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f);
	m_target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::UnitY);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(Math::HarfPI*0.5f,
		static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	m_targetMatrix = m_targetObject->GetMatrix();
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="targetObject">追従するオブジェクト</param>
/// <param name="eye">視点の相対座標</param>
/// <param name="target">注視点の相対座標</param>
/// <param name="up">カメラの上方向</param>
/// <param name="fov">画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearPlane">前面クリップ面</param>
/// <param name="farPlane">後方クリップ面</param>
TargetCamera::TargetCamera(IObject* targetObject, DirectX::SimpleMath::Vector3 eye,
	DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up,
	float fov, float aspectRatio, float nearPlane, float farPlane)
	: m_targetObject(targetObject)
	, m_pos()
	, m_eye(eye)
	, m_target(target)
	, m_lerpSpeed(0.5f) {
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	m_viewport= DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(0.5f, -0.5f, 1.0f)) *
		DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f)) *
		DirectX::SimpleMath::Matrix::CreateScale(DirectX::SimpleMath::Vector3(static_cast<float>(DirectX11::Get().GetWidth()),
			static_cast<float>(DirectX11::Get().GetHeight()), 1.0f));
	m_targetMatrix = m_targetObject->GetMatrix();
}

/// <summary>
/// ターゲットカメラを更新する
/// </summary>
void TargetCamera::Update() {
	m_targetMatrix = DirectX::SimpleMath::Matrix::Lerp(m_targetMatrix, m_targetObject->GetMatrix(), m_lerpSpeed);
	m_pos = DirectX::SimpleMath::Vector3::Transform(m_eye, m_targetMatrix);
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Transform(m_target, m_targetMatrix);
	//DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::UnitY, m_targetMatrix);
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_pos, target, up);
}

/// <summary>
/// ターゲットカメラの位置を取得する
/// </summary>
/// <returns>
/// 位置
/// </returns>
const DirectX::SimpleMath::Vector3& TargetCamera::GetEyePosition() const {
	return m_pos;
}
