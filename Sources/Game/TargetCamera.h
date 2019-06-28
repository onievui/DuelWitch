#pragma once
#ifndef TARGET_CAMERA_DEFINED
#define TARGET_CAMERA_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>

class IObject;

/// <summary>
/// �Ǐ]�J�����N���X
/// </summary>
class TargetCamera {
public:
	// �R���X�g���N�^
	TargetCamera(int width, int height, IObject* targetObject);
	TargetCamera(IObject* targetObject, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = DirectX::XM_PIDIV4,
		float aspectRatio = 800.0f / 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	// �^�[�Q�b�g�J�������X�V����
	void Update();
	// �^�[�Q�b�g�J�����̃r���[�s����擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const;
	// �^�[�Q�b�g�J�����̎ˉe�s����擾����
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const;
	// �^�[�Q�b�g�J�����̈ʒu���擾����
	const DirectX::SimpleMath::Vector3& GetEyePosition() const;



private:
	// �r���[�s��
	DirectX::SimpleMath::Matrix  m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix  m_proj;
	// �r���[�|�[�g�s��
	DirectX::SimpleMath::Matrix m_viewport;
	// �Ǐ]����I�u�W�F�N�g
	IObject*					 m_targetObject;
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

