#include "EffectiveEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\RandMt.h>
#include <Utils\ResourceManager.h>
#include <Utils\AudioManager.h>
#include <Utils\ConstBuffer.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera\Camera.h>
#include <Game\Magic\MagicID.h>
#include "EffectiveEffect.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectiveEffectEmitter::EffectiveEffectEmitter() {
	// ���������m�ۂ��Ă���
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->effectiveParam.particleNum);
	for (std::vector<std::unique_ptr<EffectiveEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<EffectiveEffect>();
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
void EffectiveEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const EffectParameter::effective_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->effectiveParam;

	m_transform.SetPosition(pos);
	m_lifeTime = parameter.lifeTime;

	DirectX::SimpleMath::Quaternion rotation = Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::UnitX, dir);

	for (std::vector<std::unique_ptr<EffectiveEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		// ���ˏ�Ƀ����_���ȕ��������߂�
		float z = RandMt::GetRand(2.0f) - 1.0f;
		float rad = RandMt::GetRand(Math::PI2);
		float x = sqrtf(1.0f - z * z) * std::cosf(rad);
		float y = sqrtf(1.0f - z * z) * std::sinf(rad);
		DirectX::SimpleMath::Vector3 direction(x, y, z);

		direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);

		float speed = RandMt::GetRange(parameter.minSpeed, parameter.maxSpeed);

		(*itr)->Initialize(parameter.lifeTime, direction * 0.2f, direction*speed);
	}

	// ���ʉ���炷
	ServiceLocater<AudioManager>::Get()->PlaySound(SoundID::Effective);
}

/// <summary>
/// �G�t�F�N�g�G�~�b�^�[���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
/// <param name="camera">�J����</param>
void EffectiveEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// �����x�N�g�����擾����
	m_eyeVec = camera->GetEyeVector();

	m_lifeTime -= elapsed_time;
	// ���Ԃ��؂ꂽ�玩�g�ō폜����
	if (m_lifeTime <= 0.0f) {
		m_lifeTime = 0.0f;
		m_isUsed = false;
	}

	// �G�t�F�N�g���X�V����
	for (std::vector<std::unique_ptr<EffectiveEffect>>::iterator itr = m_effects.begin();
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
/// <param name="proj">�ˉe�s��</param>
void EffectiveEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	const EffectParameter::death_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->deathParam;
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

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

	// ���̓��C�A�E�g�����蓖�Ă�
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// �I�����߂��Ƃ��̓A���t�@������������
	float t = 1.0f - (m_lifeTime / parameter.lifeTime);
	float alpha = (t < 0.8f ? 1 : 1 - (t - 0.8f) / 0.2f);
	DirectX::SimpleMath::Color color = m_color;
	color.w *= alpha;

	// ���_�����쐬����
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	vertex.reserve(m_effects.size());
	for (std::vector<std::unique_ptr<EffectiveEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		vertex.emplace_back(DirectX::VertexPositionColorTexture(
			(*itr)->GetPos(), color,
			DirectX::SimpleMath::Vector2((*itr)->GetScale(), (*itr)->GetAngle()) // x���X�P�[�� y��Z��]
		));
	}

	// �e�N�X�`�������蓖�Ă�
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::EffectiveEffect);
	context->PSSetShaderResources(0, 1, texture->GetResource().GetAddressOf());

	// �G�t�F�N�g��`�悷��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], vertex.size());
	batch->End();
}

/// <summary>
/// ���@ID�ŃG�t�F�N�g�̐F��ݒ肷��
/// </summary>
/// <param name="id">���@ID</param>
void EffectiveEffectEmitter::SetColorID(MagicID id) {
	switch (id) {
	case MagicID::Normal:
		m_color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case MagicID::Fire:
		m_color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case MagicID::Freeze:
		m_color = DirectX::SimpleMath::Color(0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case MagicID::Thunder:
	case MagicID::ThunderStrike:
		m_color = DirectX::SimpleMath::Color(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	default:
		ErrorMessage(L"�G�t�F�N�g�̐F�̐ݒ�ŕs���Ȓl���n����܂���");
		break;
	}
}
