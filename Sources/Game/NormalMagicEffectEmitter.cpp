#include "NormalMagicEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ConstBuffer.h>
#include <Utils\ResourceManager.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "Camera.h"
#include "NormalMagicEffect.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
NormalMagicEffectEmitter::NormalMagicEffectEmitter() {
	// ���������m�ۂ��Ă���
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->normalMagicParam.particleNum);
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<NormalMagicEffect>();
	}

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BillboardBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());

}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[�𐶐�����
/// </summary>
/// <param name="pos">�o���ʒu</param>
/// <param name="dir">����</param>
void NormalMagicEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	dir;
	m_transform.SetPosition(pos);

	// �G�t�F�N�g�̏�����
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Initialize();
	}
}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="camera">�J����</param>
void NormalMagicEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	// �����x�N�g�����擾����
	m_eyeVec = camera->GetEyeVector();

	// �G�t�F�N�g���X�V����
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Update(timer);
	}
}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[��`�悷��
/// </summary>
/// <param name="batch">�v���~�e�B�u�o�b�`</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void NormalMagicEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// �萔�o�b�t�@�̍쐬
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_pParent->GetMatrix().Transpose();
	cbuff.eye = DirectX::SimpleMath::Vector4(m_eyeVec.x, m_eyeVec.y, m_eyeVec.z, 0);
	cbuff.lookAt = false;
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = states->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɎQ�Ƃ���
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// ���v���J�����O
	context->RSSetState(states->CullClockwise());

	// �V�F�[�_�ɒ萔�o�b�t�@�����蓖�Ă�
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->GSSetConstantBuffers(0, 1, cb);

	// �s�N�Z���V�F�[�_�ɃT���v���[�����蓖�Ă�
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	// �e�V�F�[�_�����蓖�Ă�
	VertexShaderResource* vertex_shader = ServiceLocater<ResourceManager<VertexShaderResource>>::Get()
		->GetRawResource(VertexShaderID::Default);
	const GeometryShaderResource* geometry_shader = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get()
		->GetResource(GeometryShaderID::Billboard);
	const PixelShaderResource* pixel_shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()
		->GetResource(PixelShaderID::Default);
	context->VSSetShader(vertex_shader->GetResource().Get(), nullptr, 0);
	context->GSSetShader(geometry_shader->GetResource().Get(), nullptr, 0);
	context->PSSetShader(pixel_shader->GetResource().Get(), nullptr, 0);
	// �e�N�X�`�������蓖�Ă�
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Particle);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// ���̓��C�A�E�g�����蓖�Ă�
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// ���_�����쐬����
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	const float partice_scale = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->normalMagicParam.scale;
	for (std::vector<std::unique_ptr<NormalMagicEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), DirectX::Colors::White,
			DirectX::SimpleMath::Vector2(partice_scale, 0) // x���X�P�[�� y��Z��]
		));
	}
	
	// �G�t�F�N�g��`�悷��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}
