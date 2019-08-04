#include "FixCamera.h"


FixCamera::FixCamera(int width, int height) {
	m_eye = DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f);
	m_target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::UnitY);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(3.14f / 4.0f,
		static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}

FixCamera::FixCamera(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
	DirectX::SimpleMath::Vector3 up, float fov, float aspectRatio, float nearPlane, float farPlane)
	: m_eye(eye)
	, m_target(target) {
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
}

void FixCamera::Update() {
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::UnitY);
}

DirectX::SimpleMath::Matrix FixCamera::GetViewMatrix() const {
	return m_view;
}

DirectX::SimpleMath::Matrix FixCamera::GetProjectionMatrix() const {
	return m_proj;
}

DirectX::SimpleMath::Vector3 FixCamera::GetEyePosition() const {
	return m_eye;
}

void FixCamera::SetEyePosition(DirectX::SimpleMath::Vector3 eye) {
	m_eye = eye;;
}

void FixCamera::SetTargetPosition(DirectX::SimpleMath::Vector3 target) {
	m_target = target;
}
