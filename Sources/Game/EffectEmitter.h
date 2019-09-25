#pragma once
#ifndef EFFECT_EMITTER_DEFINED
#define EFFECT_EMITTER_DEFINED


#include "IEffectEmitter.h"
#include "Transform.h"


/// <summary>
/// �G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class EffectEmitter : public IEffectEmitter {
public:
	// �R���X�g���N�^
	EffectEmitter() 
		: m_inputLayout()
		, m_cBuffer()
		, m_transform()
		, m_isUsed(false)
		, m_lifeTime(0) {
	}

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override { pos, dir; }
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	virtual void Update(const DX::StepTimer& timer, const Camera* camera) override { timer, camera; }
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override { view, proj; }

public:
	// �G�t�F�N�g���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const override { return m_isUsed; }
	// �G�t�F�N�g���g�p���邩�ǂ����ݒ肷��
	void IsUsed(bool isUsed) override { m_isUsed = isUsed; }

private:
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//  �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>      m_cBuffer;
	// �p��
	Transform                                 m_transform;
	// �g�p���Ă��邩�ǂ���
	bool                                      m_isUsed;
	// ��������
	float                                     m_lifeTime;
};


#endif // !EFFECT_EMITTER_DEFINED
