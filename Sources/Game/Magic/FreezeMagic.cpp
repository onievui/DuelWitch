#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>

#include <Utils\ConstBuffer.h>
#include <Game\Camera\TargetCamera.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic()
	, m_pPlayerPos()
	, m_time()
	, m_rotateRadius() {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(IceBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// �X���@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FreezeMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.radius);
	m_pPlayerPos = &pos;
	m_time = 0.0f;
	m_rotateRadius = 0.0f;
	m_color = DirectX::Colors::SkyBlue + DirectX::SimpleMath::Color(0, 0, 0, 0.8f);
	// ������XZ�����ɕϊ�����
	m_vel = dir;
	m_vel.y = 0.0f;

	m_lifeTime = parameter.lifeTime;
}

/// <summary>
/// �X���@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	// �������Ԃ������Ȃ�������ł���
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}

	// �^�C�}�[��i�߂�
	m_time += elapsed_time;

	// �v���C���[�̈ʒu�𒆐S�Ƃ���
	DirectX::SimpleMath::Vector3 center_pos = *m_pPlayerPos;
	// ��]���a�����X�ɓ���̒l�ɋ߂Â���
	m_rotateRadius = Math::Lerp(m_rotateRadius, parameter.rotateRadius, 0.2f);
	// ���Ԃɉ����ĉ�]������
	DirectX::SimpleMath::Quaternion rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, m_time*parameter.rotateSpeed);
	// ��]��̈ʒu�����߂�
	DirectX::SimpleMath::Vector3 add_pos = DirectX::SimpleMath::Vector3::Transform(m_vel*m_rotateRadius, rot);
	
	// �v���C���[�̈ʒu�ɉ�]�����ʒu�𑫂�
	m_transform.SetPosition(center_pos + add_pos);

	// �I�u�W�F�N�g���g����]������
	m_transform.SetRotation(rot);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �X���@���J������
/// </summary>
void FreezeMagic::Lost() {

}

/// <summary>
/// �X���@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::FreezeMagic);

	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// �萔�o�b�t�@�̍쐬
	IceBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	DirectX::SimpleMath::Vector3 eye = ServiceLocater<TargetCamera>::Get()->GetEyeVector();
	cbuff.eye = DirectX::SimpleMath::Vector4(eye.x, eye.y, eye.z, 0);

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	// �V�F�[�_�ɒ萔�o�b�t�@�����蓖�Ă�
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	context->VSSetConstantBuffers(2, 1, cb);

	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Ice);

	resource->GetResource()->Draw(m_world, view, proj, m_color, texture->GetResource().Get(), false,[=]() {
		ID3D11BlendState* blendstate = states->NonPremultiplied();
		// �������菈��
		context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
		// �[�x�o�b�t�@�ɎQ�Ƃ���
		context->OMSetDepthStencilState(states->DepthDefault(), 0);
		// ���v���J�����O
		context->RSSetState(states->CullClockwise());

		// �s�N�Z���V�F�[�_�ɃT���v���[�����蓖�Ă�
		ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);
		// �e�V�F�[�_�����蓖�Ă�
		VertexShaderResource* vertex_shader = ServiceLocater<ResourceManager<VertexShaderResource>>::Get()
			->GetRawResource(VertexShaderID::Ice);
		const PixelShaderResource* pixel_shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()
			->GetResource(PixelShaderID::Default);

		// ���̓��C�A�E�g�����蓖�Ă�
		context->IASetInputLayout(vertex_shader->GetInputLayout());

		context->VSSetShader(vertex_shader->GetResource().Get(), nullptr, 0);
		context->PSSetShader(pixel_shader->GetResource().Get(), nullptr, 0);
	});
	//m_collider->Render(view, proj);

	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

/// <summary>
/// �_���[�W���擾����
/// </summary>
/// <returns>
/// �_���[�W��
/// </returns>
float FreezeMagic::GetPower() const {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void FreezeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void FreezeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// �����@�ƏՓ˂����������
	if (other_id == MagicID::Fire) {
		m_isUsed = false;
	}
}
