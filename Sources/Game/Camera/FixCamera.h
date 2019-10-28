#pragma once
#ifndef FIX_CAMERA_DEFINED
#define FIX_CAMERA_DEFINED


/// <summary>
/// �Ǐ]�J�����N���X
/// </summary>
class FixCamera {
public:
	// �R���X�g���N�^
	FixCamera(int width, int height);
	FixCamera(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY, float fov = DirectX::XM_PIDIV4,
		float aspectRatio = 800.0f/600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	// �Œ�J�������X�V����
	void Update();
	// �Œ�J�����̃r���[�s����擾����
	DirectX::SimpleMath::Matrix GetViewMatrix() const;
	// �Œ�J�����̎ˉe�s����擾����
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const;
	// �Œ�J�����̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetEyePosition() const;
	// �Œ�J�����̈ʒu��ݒ肷��
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye);
	// �Œ�J�����̒����_��ݒ肷��
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target);

private:
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// �^�[�Q�b�g�J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;

};

#endif	// FIX_CAMERA_DEFINED

