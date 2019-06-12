#pragma once
#ifndef DEBUG_CAMERA_DEFINED
#define DEBUG_CAMERA_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>
//#include <Mouse.h>

class DebugCamera {
private:
	// ���[�V����
	void Motion(int x, int y);

public:
	// �R���X�g���N�^
	DebugCamera(int width, int height);
	// �f�o�b�O�J�������X�V����
	void Update();
	// �f�o�b�O�J�����̃r���[�s����擾����
	DirectX::SimpleMath::Matrix GetCameraMatrix() const;
	// �f�o�b�O�J�����̈ʒu���擾����
	DirectX::SimpleMath::Vector3 GetEyePosition() const;
	// �����_�̈ʒu��Ԃ�
	DirectX::SimpleMath::Vector3 GetTargetPosition() const;
	// �E�B���h�E�X�P�[���𒲐�����
	void AdjustWindowScale(int width, int height);

private:
	// �J�����̋���
	static const float CAMERA_DISTANCE;
	// ����]
	float m_yAngle, m_yTmp;
	// �c��]
	float m_xAngle, m_xTmp;
	// �h���b�O���ꂽ���W
	int m_x, m_y;
	// �X�P�[��
	float m_xScale, m_yScale;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �X�N���[���z�C�[���l
	int m_scrollWheelValue;
	// ���_
	DirectX::SimpleMath::Vector3 m_eye;
	// �����_
	DirectX::SimpleMath::Vector3 m_target;
	// �}�E�X�g���b�J�[
	DirectX::Mouse::ButtonStateTracker m_tracker;
};

#endif	// DEBUG_CAMERA_DEFINED

