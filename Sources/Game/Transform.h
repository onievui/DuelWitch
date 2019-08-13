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
		const DirectX::SimpleMath::Quaternion& rotation = DirectX::SimpleMath::Quaternion::Identity,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);
	// �R���X�g���N�^
	Transform(const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& euler,
		const DirectX::SimpleMath::Vector3& scale = DirectX::SimpleMath::Vector3::One);

public:
	// ���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition() const           { return m_position; }
	// ���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& position)    { m_position = position; }
	// ��]���擾����
	const DirectX::SimpleMath::Quaternion& GetRotation() const        { return m_rotation; }
	// ��]��ݒ肷��
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) { m_rotation = rotation; }
	// �X�P�[�����擾����
	const DirectX::SimpleMath::Vector3& GetScale() const              { return m_scale; }
	// �X�P�[����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)          { m_scale = scale; }

	// ���[���h�s����擾����
	DirectX::SimpleMath::Matrix GetMatrix() const;

private:
	// ���W
	DirectX::SimpleMath::Vector3    m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3    m_scale;

};



#endif // !TRANSFORM_DEFINED
