#include "PlayerChargeEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ResourceManager.h>
#include <Utils\ConstBuffer.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera\Camera.h>
#include "PlayerChargeEffect.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerChargeEffectEmitter::PlayerChargeEffectEmitter() {
	// ���������m�ۂ��Ă���
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam.particleNum);
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<PlayerChargeEffect>();
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
/// �G�t�F�N�g�𐶐�����
/// </summary>
/// <param name="pos"></param>
/// <param name="dir"></param>
void PlayerChargeEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	
	m_chargeState = State::None;
	m_transform.SetPosition(pos);
	
	float delay_interval = parameter.lifeTime / m_effects.size();
	float interval = 0.0f;

	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Initialize(parameter.lifeTime, parameter.appearPosOffset, dir, dir);
		(*itr)->SetDelayTime(interval);
		interval += delay_interval;
	}
}

/// <summary>
/// �G�t�F�N�g���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="camera">�J����</param>
void PlayerChargeEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	// �����x�N�g�����擾����
	m_eyeVec = camera->GetEyeVector();

	// �G�t�F�N�g���X�V����
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Update(timer);
	}
}

/// <summary>
/// �G�t�F�N�g��`�悷��
/// </summary>
/// <param name="batch">�v���~�e�B�u�o�b�`</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void PlayerChargeEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	const EffectParameter::player_charge_param parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// �`���[�W��ԂłȂ��Ȃ�`�悵�Ȃ�
	if (m_chargeState == State::None) {
		return;
	}

	// �萔�o�b�t�@�̍쐬
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_transform.GetMatrix().Transpose();
	cbuff.eye = DirectX::SimpleMath::Vector4(m_eyeVec.x, m_eyeVec.y, m_eyeVec.z, 0);
	cbuff.lookAt = false;
	cbuff.diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// �������菈��
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
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

	// ���̓��C�A�E�g�����蓖�Ă�
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// ���_�����쐬����
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	vertex.reserve(m_effects.size());
	for (std::vector<std::unique_ptr<PlayerChargeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		// �J�n���Ă��Ȃ��G�t�F�N�g�͔�΂�
		if ((*itr)->IsWaiting()) {
			continue;
		}
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), GetColor(),
			DirectX::SimpleMath::Vector2((*itr)->GetScale(), 0.0f) // x���X�P�[�� y��Z��]
		));
	}

	// �e�N�X�`�������蓖�Ă�
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::PlayerCharge);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// �G�t�F�N�g��`�悷��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();

}

/// <summary>
/// �e�I�u�W�F�N�g��ݒ肷��
/// </summary>
/// <param name="pParent">�e�I�u�W�F�N�g�ւ̃|�C���^</param>
void PlayerChargeEffectEmitter::SetParent(const Transform* pParent) {
	m_pParent = pParent;
	m_transform.SetParent(pParent);
}

/// <summary>
/// �F���擾����
/// </summary>
/// <returns>
/// �G�t�F�N�g�̐F
/// </returns>
DirectX::SimpleMath::Color PlayerChargeEffectEmitter::GetColor() const {
	const EffectParameter::player_charge_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->playerChargeParam;
	DirectX::SimpleMath::Color color;
	// �`���[�W��Ԃɂ���ĐF�𕪂���
	switch (m_chargeState) {
	case PlayerChargeEffectEmitter::State::Charge0:
		color = parameter.colorCharge0;
		break;
	case PlayerChargeEffectEmitter::State::Charge1:
		color = parameter.colorCharge1;
		break;
	case PlayerChargeEffectEmitter::State::Charge2:
		color = parameter.colorCharge2;
		break;
	default:
		ErrorMessage(L"�v���C���[�̃`���[�W�G�t�F�N�g�̏�Ԃ��s���ł�");
		break;
	}

	return color / 255.0f;
}