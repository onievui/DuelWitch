#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED

//#include "DirectX11.h"
//#include "Model.h"
//#include "CommonStates.h"
//#include "SimpleMath.h"

/// <summary>
/// �t�B�[���h�N���X
/// </summary>
class Field {
public:
	Field();
	~Field();

	void Update();
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const;

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skydome;

};




#endif // !FIELD_DEFINED
