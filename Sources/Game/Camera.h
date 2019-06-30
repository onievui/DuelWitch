#pragma once
#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED


/// <summary>
/// �J�����N���X
/// </summary>
class Camera {
public:
	// �J�����̃r���[�s����擾����
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const {
		return m_view;
	}
	// �J�����̎ˉe�s����擾����
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const {
		return m_proj;
	}
	// �J�����̈ʒu���擾����
	virtual const DirectX::SimpleMath::Vector3& GetEyePosition() const {
		return m_eye;
	}
	// �J�����̈ʒu��ݒ肷��
	virtual void SetEyePosition(DirectX::SimpleMath::Vector3 eye) {
		m_eye = eye;
	}
	// �J�����̒����_��ݒ肷��
	virtual void SetTargetPosition(DirectX::SimpleMath::Vector3 target) {
		m_target = target;
	}

public:
	// �X�N���[�����W���烌�C�ɕϊ�����
	DirectX::SimpleMath::Ray ScreenPointToRay(const DirectX::SimpleMath::Vector3& point) const;
	// �r���[�|�[�g���W���烌�C�ɕϊ�����
	DirectX::SimpleMath::Ray ViewportPointToRay(const DirectX::SimpleMath::Vector3& point) const;

	// ���[���h���W����X�N���[�����W�ɕϊ�����
	DirectX::SimpleMath::Vector3 WorldToScreenPoint(const DirectX::SimpleMath::Vector3& point) const;
	// ���[���h���W����r���[�|�[�g���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 WorldToViewportPoint(const DirectX::SimpleMath::Vector3& point) const;
	// �r���[�|�[�g���W���烏�[���h���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ViewportToWorldPoint(const DirectX::SimpleMath::Vector3& point) const;
	// �X�N���[�����W���烏�[���h���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ScreenToWorldPoint(const DirectX::SimpleMath::Vector3& point) const;
	// �X�N���[�����W����r���[�|�[�g���W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ScreenToViewportPoint(const DirectX::SimpleMath::Vector3& point) const;
	// �r���[�|�[�g���W����X�N���[�����W�ɕϊ�����
	DirectX::SimpleMath::Vector3 ViewportToScreenPoint(const DirectX::SimpleMath::Vector3& point) const;

protected:
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eye;
	// �r���[�s��
	DirectX::SimpleMath::Matrix  m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix  m_proj;
	// �r���[�|�[�g�s��
	DirectX::SimpleMath::Matrix  m_viewport;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
};


#endif // !CAMERA_DEFINED
