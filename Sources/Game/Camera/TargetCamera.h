#pragma once
#ifndef TARGET_CAMERA_DEFINED
#define TARGET_CAMERA_DEFINED


#include "Camera.h"
#include <Game\Object\Transform.h>


class IObject;


/// <summary>
/// �Ǐ]�J�����N���X
/// </summary>
class TargetCamera : public Camera {
public:
	// �R���X�g���N�^
	TargetCamera(const IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up, PerspectiveFovInfo perspectiveFovInfo);

public:
	// �^�[�Q�b�g�J�������X�V����
	void Update();
	// �Ǐ]����I�u�W�F�N�g��ݒ肷��
	void SetTargetObject(const IObject* pTargetObject) { m_pTargetObject = pTargetObject; }
	// �Ǐ]����I�u�W�F�N�g�����݂��邩�ǂ����擾����
	bool HasTargetObject();
	// ��p��ݒ肷��
	void SetFov(float fov);
	// ��p���擾����
	float GetFov() const { return m_perspectiveFovInfo.m_fov; }
	// ���ڑ��삷�邽�߂̍s���ݒ肷��
	void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { m_matrix = matrix; }

private:
	// �Ǐ]����I�u�W�F�N�g
	const IObject*				 m_pTargetObject;
	DirectX::SimpleMath::Matrix  m_targetMatrix;
	// ���_�̑��Έʒu
	DirectX::SimpleMath::Vector3 m_relativeEye;
	// �����_�̑��Έʒu
	DirectX::SimpleMath::Vector3 m_relativeTarget;
	// �Ǐ]���x
	float                        m_lerpSpeed;
	// �������e�Ɋւ�����
	PerspectiveFovInfo           m_perspectiveFovInfo;
	// ���ڑ��삷�邽�߂̍s��
	DirectX::SimpleMath::Matrix  m_matrix;
};

#endif	// TARGET_CAMERA_DEFINED
