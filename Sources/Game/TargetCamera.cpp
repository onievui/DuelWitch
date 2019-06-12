#include "TargetCamera.h"
#include "ModelObject.h"


TargetCamera::TargetCamera(int width, int height, ModelObject* targetObject) 
	: m_targetObject(targetObject)
	, m_pos()
	, m_lerpSpeed(0.05f) {
	m_eye = DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f);
	m_target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::UnitY);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(Math::HarfPI*0.5f,
		float(width) / float(height), 0.1f, 100.0f);
	m_targetMatrix = m_targetObject->GetMatrix();
}

TargetCamera::TargetCamera(ModelObject* targetObject, DirectX::SimpleMath::Vector3 eye,
	DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up,
	float fov, float aspectRatio, float nearPlane, float farPlane)
	: m_targetObject(targetObject)
	, m_pos()
	, m_eye(eye)
	, m_target(target)
	, m_lerpSpeed(0.5f) {
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	m_targetMatrix = m_targetObject->GetMatrix();
}

void TargetCamera::Update() {
	m_targetMatrix = DirectX::SimpleMath::Matrix::Lerp(m_targetMatrix, m_targetObject->GetMatrix(), m_lerpSpeed);
	m_pos = DirectX::SimpleMath::Vector3::Transform(m_eye, m_targetMatrix);
	auto target = DirectX::SimpleMath::Vector3::Transform(m_target, m_targetMatrix);
	//auto up = DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::UnitY, m_targetMatrix);
	auto up = DirectX::SimpleMath::Vector3::UnitY;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_pos, target, up);
}

DirectX::SimpleMath::Matrix TargetCamera::GetViewMatrix() const {
	return m_view;
}

DirectX::SimpleMath::Matrix TargetCamera::GetProjectionMatrix() const {
	return m_proj;
}

DirectX::SimpleMath::Vector3 TargetCamera::GetEyePosition() const {
	return m_pos;
}
