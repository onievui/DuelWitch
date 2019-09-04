#pragma once
#ifndef TARGET_CAMERA_DEFINED
#define TARGET_CAMERA_DEFINED


#include "Camera.h"


class IObject;


/// <summary>
/// �Ǐ]�J�����N���X
/// </summary>
class TargetCamera : public Camera {
public:
	// �R���X�g���N�^
	TargetCamera(int width, int height, IObject* targetObject);
	TargetCamera(IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = DirectX::XM_PIDIV4,
		float aspectRatio = 800.0f / 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

public:
	// �^�[�Q�b�g�J�������X�V����
	void Update();
	// �^�[�Q�b�g�J�����̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const override { return m_pos; }
	// �Ǐ]����I�u�W�F�N�g��ݒ肷��
	void SetTargetObject(IObject* pTargetObject) { m_pTargetObject = pTargetObject; }
	// �Ǐ]����I�u�W�F�N�g�����݂��邩�ǂ����擾����
	bool HasTargetObject();

private:
	// �Ǐ]����I�u�W�F�N�g
	IObject*					 m_pTargetObject;
	DirectX::SimpleMath::Matrix  m_targetMatrix;
	// �^�[�Q�b�g�J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_pos;
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �Ǐ]���x
	float                        m_lerpSpeed;
};

#endif	// TARGET_CAMERA_DEFINED

