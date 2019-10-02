#pragma once
#ifndef FIELD_DEFINED
#define FIELD_DEFINED


#include <map>
#include <Framework\StepTimer.h>
#include "Transform.h"


class Player;
class FieldShieldEffectEmitter;


/// <summary>
/// �t�B�[���h�N���X
/// </summary>
class Field {
public:
	// �R���X�g���N�^
	Field();
	// �f�X�g���N�^
	~Field();

public:
	// �t�B�[���h���X�V����
	void Update(const DX::StepTimer& timer);
	// �t�B�[���h��`�悷��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// �v���C���[�ƃt�B�[���h�̓����蔻����s��
	void CollisionCheckPlayer(Player& player);

private:
	// �t�B�[���h�Փ˃G�t�F�N�g�𐶐�����
	void CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos);
	// �ڈ�ƂȂ�摜��`�悷��
	void DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                                   m_states;
	// �X�J�C�h�[��
	std::unique_ptr<DirectX::Model>                                          m_skydome;
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect>                                m_batchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                                m_inputLayout;

	// U�^�[���摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                         m_turnTexture;

	// �p��
	Transform                                                                m_transform;
	// �t�B�[���h�O��
	std::unique_ptr<DirectX::GeometricPrimitive>                             m_wall;
	// �t�B�[���h���a
	float                                                                    m_radius;

	// �t�B�[���h�Փ˃G�t�F�N�g
	FieldShieldEffectEmitter*                                                m_pEffect;
	// �G�t�F�N�g�𔭐������邩�ǂ����̔���p�^�C�}�[
	std::map<const Player*, float>                                           m_effectTimer;
};




#endif // !FIELD_DEFINED
