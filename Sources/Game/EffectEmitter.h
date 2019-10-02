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
		: m_cBuffer()
		, m_transform(nullptr)
		, m_pParent()
		, m_isUsed(false)
		, m_lifeTime(0) {
	}

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override { pos, dir; }
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	virtual void Update(const DX::StepTimer& timer, const Camera* camera) override { timer, camera; }
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	virtual void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override
	{ batch, view, proj; }

public:
	// �e��ݒ肷��
	void SetParent(const Transform* parent) override { m_pParent = parent; }
	// �G�t�F�N�g���g�p���Ă��邩�ǂ����擾����
	bool IsUsed() const override { return m_isUsed; }
	// �G�t�F�N�g���g�p���邩�ǂ����ݒ肷��
	void SetUsed(bool isUsed) override { m_isUsed = isUsed; }

protected:
	//  �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>      m_cBuffer;
	// �p��
	ChildTransform                            m_transform;
	// �e�̎p��
	const Transform*                          m_pParent;
	// �g�p���Ă��邩�ǂ���
	bool                                      m_isUsed;
	// ��������
	float                                     m_lifeTime;
};


#endif // !EFFECT_EMITTER_DEFINED
