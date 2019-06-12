#pragma once
#ifndef TRANSFORM_DEFINED
#define TRANSFORM_DEFINED

//#include "DirectX11.h"
//#include <SimpleMath.h>


/// <summary>
/// �p���N���X
/// </summary>
class Transform {
public:
	// �R���X�g���N�^
	Transform(const DirectX::SimpleMath::Vector3& position = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& rotation = DirectX::SimpleMath::Vector3::Zero,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition() const;
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position);
	// ��]���擾����
	const DirectX::SimpleMath::Vector3& GetRotation() const;
	// ��]��ݒ肷��
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation);
	// �X�P�[�����擾����
	const DirectX::SimpleMath::Vector3& GetScale() const;
	// �X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale);

	// ���[���h�s����擾����
	DirectX::SimpleMath::Matrix GetMatrix() const;

private:
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Vector3 m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;

};



#endif // !TRANSFORM_DEFINED
