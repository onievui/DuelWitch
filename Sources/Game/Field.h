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

public:
	void Update();
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	void DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model> m_skydome;
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// U�^�[���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_turnTexture;

};




#endif // !FIELD_DEFINED
