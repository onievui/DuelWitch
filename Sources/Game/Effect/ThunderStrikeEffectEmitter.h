#pragma once
#ifndef THUNDER_STRIKE_EFFECT_EMITTER_DEFINED
#define THUNDER_STRIKE_EFFECT_EMITTER_DEFINED


#include "EffectEmitter.h"


class ThunderStrikeEffect;
struct BlurParam;


/// <summary>
/// �ʏ햂�@�G�t�F�N�g�G�~�b�^�[�N���X
/// </summary>
class ThuderStrikeEffectEmitter : public EffectEmitter {
public:
	// �R���X�g���N�^
	ThuderStrikeEffectEmitter();

public:
	// �G�t�F�N�g�G�~�b�^�[�𐶐�����
	void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) override;
	// �G�t�F�N�g�G�~�b�^�[���X�V����
	void Update(const DX::StepTimer& timer, const Camera* camera) override;
	// �G�t�F�N�g�G�~�b�^�[��`�悷��
	void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

private:
	// �K�E�X�̏d�݂��v�Z����
	float GaussianDistribution(const DirectX::SimpleMath::Vector2& pos, float rho);
	// �u���[�p�p�����[�^���v�Z����
	BlurParam CalcBlurParam(int width, int height, const DirectX::SimpleMath::Vector2& dir, float deviation, float multiply);

private:
	// �G�t�F�N�g
	std::vector<std::unique_ptr<ThunderStrikeEffect>> m_effects;
	// �����x�N�g��
	DirectX::SimpleMath::Vector3 m_eyeVec;
	// �s�N�Z���V�F�[�_�p�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>      m_cBufferPixel;
	// �^�C���f�[�^
	DirectX::SimpleMath::Vector4 m_time;
	// �u���[�̕���
	DirectX::SimpleMath::Vector2 m_blurDirection;
};


#endif // !THUNDER_STRIKE_EFFECT_EMITTER_DEFINED
