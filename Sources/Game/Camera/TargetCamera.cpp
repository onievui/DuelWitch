#include "TargetCamera.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Game\Object\IObject.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="targetObject">�Ǐ]����I�u�W�F�N�g</param>
/// <param name="eye">���_�̑��΍��W</param>
/// <param name="target">�����_�̑��΍��W</param>
/// <param name="up">�J�����̏����</param>
/// <param name="perspectiveFovInfo">�������e�Ɋւ�����</param>
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
/// �^�[�Q�b�g�J�������X�V����
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
/// �Ǐ]����I�u�W�F�N�g�����݂��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : ���݂���
/// false : ���݂��Ȃ�
/// </returns>
bool TargetCamera::HasTargetObject() {
	return m_pTargetObject != nullptr;
}

/// <summary>
/// ��p��ݒ肷��
/// </summary>
/// <param name="fov">��p</param>
void TargetCamera::SetFov(float fov) {
	m_perspectiveFovInfo.m_fov = fov;
	m_proj = m_perspectiveFovInfo.CreateMatrix();
}

