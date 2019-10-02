#include "FieldShieldEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ResourceManager.h>
#include <Utils\ConstBuffer.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "FieldShieldEffect.h"
#include "Camera.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FieldShieldEffectEmitter::FieldShieldEffectEmitter() {
	// ���������m�ۂ��Ă���
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam.particleNum);
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<FieldShieldEffect>();
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
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FieldShieldEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// AddEffcet�֐�����G�t�F�N�g��ǉ�����
	pos; dir;
}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="camera">�J����</param>
void FieldShieldEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	camera;

	// �G�t�F�N�g���X�V����
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		if ((*itr)->IsUsed()) {
			(*itr)->Update(timer);
		}
	}
}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[��`�悷��
/// </summary>
/// <param name="batch">�v���~�e�B�u�o�b�`</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FieldShieldEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// ���_�����쐬����
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	for (std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		if (!(*itr)->IsUsed()) {
			continue;
		}
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), (*itr)->GetColor(),
			DirectX::SimpleMath::Vector2((*itr)->GetScale(), 0) // x���X�P�[�� y��Z��]
		));
	}

	// �G�t�F�N�g���o�����Ă��Ȃ��ꍇ�͏������Ȃ�
	if (vertex.size() == 0) {
		return;
	}

	// �萔�o�b�t�@�̍쐬
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = DirectX::SimpleMath::Matrix::Identity;
	const DirectX::SimpleMath::Vector3& eye_pos = m_pParent->GetLocalPosition();
	cbuff.eye = DirectX::SimpleMath::Vector4(eye_pos.x, eye_pos.y, eye_pos.z, 0);
	cbuff.lookAt = true;
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
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::FieldShield);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// ���̓��C�A�E�g�����蓖�Ă�
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// �G�t�F�N�g��`�悷��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}

/// <summary>
/// �G�t�F�N�g��ǉ�����
/// </summary>
/// <param name="pos">���W</param>
/// <param name="normal">����</param>
void FieldShieldEffectEmitter::AddEffect(const DirectX::SimpleMath::Vector3& pos) {
	// ���g�p�̃G�t�F�N�g��T��
	std::vector<std::unique_ptr<FieldShieldEffect>>::iterator itr =
		LamdaUtils::FindIfNot(m_effects, LamdaUtils::GetLamda(&FieldShieldEffect::IsUsed));

	if (itr == m_effects.end()) {
		return;
	}

	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;

	// ���������Ēǉ�
	(*itr)->Initialize(parameter.lifeTime, pos);
}
