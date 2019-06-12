#pragma once
#ifndef MODEL_OBJECT_DEFINED
#define MODEL_OBJECT_DEFINED

#include <Framework\StepTimer.h>
//#include "DirectX11.h"
//#include <string>
//#include "StepTimer.h"
//#include <SimpleMath.h>
//#include <Effects.h>
//#include <CommonStates.h>
//#include <Model.h> 

#include "Transform.h"

/// <summary>
/// ���f���I�u�W�F�N�g�N���X
/// </summary>
class ModelObject {
public:
	ModelObject();
	~ModelObject();

	// ���f���I�u�W�F�N�g���X�V����
	void Update(const DX::StepTimer& timer);
	// ���f���I�u�W�F�N�g���J������
	void Lost();
	// ���f���I�u�W�F�N�g�𐶐�����
	void Create(const std::wstring& fileName, const std::wstring& directory = L"");
	// ���f���I�u�W�F�N�g��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

public:
	// ���f���I�u�W�F�N�g�̍s����擾����
	const DirectX::SimpleMath::Matrix& GetMatrix() const;

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// ���f��
	std::unique_ptr<DirectX::Model>        m_model;
	// �p��
	Transform                              m_transform;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix            m_world;
};


#endif // !MODEL_OBJECT_DEFINED


