#include "TargetCamera.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include "IObject.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="width">��ʂ̉���</param>
/// <param name="height">��ʂ̏c��</param>
/// <param name="targetObject">�Ǐ]����I�u�W�F�N�g</param>
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
/// �R���X�g���N�^
/// </summary>
/// <param name="targetObject">�Ǐ]����I�u�W�F�N�g</param>
/// <param name="eye">���_�̑��΍��W</param>
/// <param name="target">�����_�̑��΍��W</param>
/// <param name="up">�J�����̏����</param>
/// <param name="fov">��p</param>
/// <param name="aspectRatio">�A�X�y�N�g��</param>
/// <param name="nearPlane">�O�ʃN���b�v��</param>
/// <param name="farPlane">����N���b�v��</param>
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
/// �^�[�Q�b�g�J�������X�V����
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
/// �^�[�Q�b�g�J�����̈ʒu���擾����
/// </summary>
/// <returns>
/// �ʒu
/// </returns>
const DirectX::SimpleMath::Vector3& TargetCamera::GetEyePosition() const {
	return m_pos;
}
