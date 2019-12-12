#include "ThunderStrikeEffectEmitter.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ConstBuffer.h>
#include <Utils\ResourceManager.h>
#include <Parameters\EffectParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Camera\Camera.h>
#include "ThunderStrikeEffect.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ThunderStrikeEffectEmitter::ThunderStrikeEffectEmitter() {
	// ���������m�ۂ��Ă���
	m_effects.resize(ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->thunderStrikeMagicParam.particleNum);
	for (std::vector<std::unique_ptr<ThunderStrikeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		*itr = std::make_unique<ThunderStrikeEffect>();
	}

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BillboardBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());

	bd.ByteWidth = sizeof(ThunderStrikeBuffer);
	
	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBufferPixel.GetAddressOf());
}


void ThunderStrikeEffectEmitter::Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	dir;
	m_transform.SetPosition(pos);

	// �G�t�F�N�g�̏�����
	for (std::vector<std::unique_ptr<ThunderStrikeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Initialize();
	}

	m_textureIndex = 0;
}

void ThunderStrikeEffectEmitter::Update(const DX::StepTimer& timer, const Camera* camera) {
	// Y����]�݂̂ɂ��邽�߁AY���W�����낦�Č������擾����
	//m_eyeVec = camera->GetEyePosition() - m_pParent->GetPosition();
	m_eyeVec = camera->GetEyeVector();
	m_eyeVec.y = 0.0f;
	m_eyeVec.Normalize();

	// �^�C���f�[�^���擾���� x:�o�ߎ���(�g�[�^���b) y:1F�̌o�ߎ���(�b) z:�T�C���J�[�u w:���g�p
	m_time = DirectX::SimpleMath::Vector4(
		static_cast<float>(timer.GetTotalSeconds()),
		static_cast<float>(timer.GetElapsedSeconds()),
		static_cast<float>(std::sin(timer.GetTotalSeconds())),
		1
	);

	// �G�t�F�N�g���X�V����
	for (std::vector<std::unique_ptr<ThunderStrikeEffect>>::iterator itr = m_effects.begin();
		itr != m_effects.end();
		++itr) {
		(*itr)->Update(timer);
	}

	// �A�j���[�V�����p�̉摜�C���f�b�N�X���X�V����
	m_textureIndex = static_cast<int>(std::floor(m_time.x * 16)) % 4;
}

/// <summary>
///	�G�t�F�N�g�G�~�b�^�[��`�悷��
/// </summary>
/// <param name="batch">�v���~�e�B�u�o�b�`</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void ThunderStrikeEffectEmitter::Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	DirectX11* directX = ServiceLocater<DirectX11>::Get();
	ID3D11DeviceContext* context = directX->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

	// �萔�o�b�t�@�̍쐬
	BillboardBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_pParent->GetMatrix().Transpose();
	cbuff.eye = DirectX::SimpleMath::Vector4(m_eyeVec.x, m_eyeVec.y, m_eyeVec.z, 0);
	cbuff.lookAt = false;
	cbuff.diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

	ThunderStrikeBuffer cbuff2;
	// TODO: �u���[�p�����[�^�̒萔���O������ǂݍ���
	cbuff2.blurParam = CalcBlurParam(directX->GetWidth(), directX->GetHeight(), m_blurDirection, 5.0f, 2.0f);
	cbuff2.time = m_time;


	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	context->UpdateSubresource(m_cBufferPixel.Get(), 0, NULL, &cbuff2, 0, 0);

	ID3D11BlendState* blendstate = states->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɎQ�Ƃ���
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// ���v���J�����O
	context->RSSetState(states->CullClockwise());

	// �V�F�[�_�ɒ萔�o�b�t�@�����蓖�Ă�
	ID3D11Buffer* cb[2] = { m_cBuffer.Get(), m_cBufferPixel.Get() };
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(1, 1, cb + 1);

	// �s�N�Z���V�F�[�_�ɃT���v���[�����蓖�Ă�
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	// �e�V�F�[�_�����蓖�Ă�
	VertexShaderResource* vertex_shader = ServiceLocater<ResourceManager<VertexShaderResource>>::Get()
		->GetRawResource(VertexShaderID::Default);
	const GeometryShaderResource* geometry_shader = ServiceLocater<ResourceManager<GeometryShaderResource>>::Get()
		->GetResource(GeometryShaderID::Billboard);
	const PixelShaderResource* pixel_shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()
		->GetResource(PixelShaderID::ThunderStrikeMagic);
	context->VSSetShader(vertex_shader->GetResource().Get(), nullptr, 0);
	context->GSSetShader(geometry_shader->GetResource().Get(), nullptr, 0);
	context->PSSetShader(pixel_shader->GetResource().Get(), nullptr, 0);

	// �e�N�X�`�������蓖�Ă�
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::ThunderStrikeMagicEffect);
	context->PSSetShaderResources(0, 1, texture->GetResource(m_textureIndex).GetAddressOf());

	// ���̓��C�A�E�g�����蓖�Ă�
	context->IASetInputLayout(vertex_shader->GetInputLayout());

	// ���_�����쐬����
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	const float partice_scale = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->thunderStrikeMagicParam.scale;
	for (std::vector<std::unique_ptr<ThunderStrikeEffect>>::iterator itr = m_effects.begin();
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

/// <summary>
/// �K�E�X�̏d�݂��v�Z����
/// </summary>
/// <param name="pos">���΍��W</param>
/// <param name="rho">�΍�</param>
/// <returns>
/// �d��
/// </returns>
float ThunderStrikeEffectEmitter::GaussianDistribution(const DirectX::SimpleMath::Vector2& pos, float rho) {
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

/// <summary>
/// �K�E�X�p�����[�^���v�Z����
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
/// <param name="dir">�u���[�̕���</param>
/// <param name="deviation">�΍�</param>
/// <param name="multiply">�K�E�X�̏d�݂̔{��</param>
/// <returns>
/// �K�E�X�p�����[�^
/// </returns>
BlurParam ThunderStrikeEffectEmitter::CalcBlurParam(int width, int height, const DirectX::SimpleMath::Vector2& dir, float deviation, float multiply) {
	BlurParam result;
	result.sampleCount = 16;
	auto tu = 1.0f / float(width);
	auto tv = 1.0f / float(height);

	result.offset[0].z = GaussianDistribution(DirectX::SimpleMath::Vector2(0.0f, 0.0f), deviation) * multiply;
	auto total_weight = result.offset[0].z;

	result.offset[0].x = 0.0f;
	result.offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		result.offset[i].x = dir.x * i * tu;
		result.offset[i].y = dir.y * i * tv;
		result.offset[i].z = GaussianDistribution(dir * float(i), deviation) * multiply;
		total_weight += result.offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		result.offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		result.offset[i].x = -result.offset[i - 7].x;
		result.offset[i].y = -result.offset[i - 7].y;
		result.offset[i].z = result.offset[i - 7].z;
	}

	return result;
}
