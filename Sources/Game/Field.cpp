#include "Field.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "Player.h"
#include "EffectManager.h"
#include "EffectID.h"
#include "FieldShieldEffectEmitter.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field() {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// �R�����X�e�[�g���쐬����
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// �G�t�F�N�g�t�@�N�g���[���쐬����
	std::unique_ptr<DirectX::EffectFactory> fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// CMO��ǂݍ���ŃX�J�C�h�[�����쐬����
	m_skydome = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Protected/skydome.cmo", *fxFactory);

	m_skydome->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			// ���C�g�̉e�����Ȃ��� 
			lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
			lights->SetLightEnabled(0, false);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
		DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basicEffect) {
			// �G�~�b�V�����F�𔒂ɐݒ肷�� 
			basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
		}
	});

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	//�e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Protected/u_turn.png", nullptr, m_turnTexture.GetAddressOf());

	// �t�B�[���h�̏���������
	m_radius = 80.0f;
	m_transform.SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));

	// �ǂ̐���
	m_wall = DirectX::GeometricPrimitive::CreateSphere(context, m_radius*2.0f, 8U, false, true);

	// �G�t�F�N�g�̐���
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FieldShield,
		m_transform.GetLocalPosition(), DirectX::SimpleMath::Vector3::Zero);
	effect->SetParent(&m_transform);
	m_pEffect = dynamic_cast<FieldShieldEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"�t�B�[���h�Փ˃G�t�F�N�g�̐����Ɏ��s���܂���");
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Field::~Field() {
}

/// <summary>
/// �t�B�[���h���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Field::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	// �G�t�F�N�g�p�^�C�}�[���X�V����
	for (std::map<const Player*, float>::iterator itr = m_effectTimer.begin(); itr != m_effectTimer.end(); ++itr) {
		itr->second -= elapsed_time;
	}
}

/// <summary>
/// �t�B�[���h��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void Field::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::SimpleMath::Vector3 center_pos = m_transform.GetLocalPosition();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);
	m_skydome->Draw(context, *m_states, world, view, proj);

	world = DirectX::SimpleMath::Matrix::CreateScale(4.0f);
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(Math::PI);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 3, -82);
	DrawTurn(world, view, proj);

	world = DirectX::SimpleMath::Matrix::CreateScale(4.0f);
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(0);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 3, 82);
	DrawTurn(world, view, proj);

	world = DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);
	m_wall->Draw(world, view, proj, DirectX::SimpleMath::Color(30/255.0f, 130/255.0f, 240/255.0f, 1.0f), nullptr, true);

}

/// <summary>
/// �v���C���[�ƃt�B�[���h�̓����蔻����s��
/// </summary>
/// <param name="player">�v���C���[</param>
void Field::CollisionCheckPlayer(Player& player) {
	const Collider* collider = player.GetCollider();
	DirectX::SimpleMath::Vector3 center_pos = m_transform.GetLocalPosition();
	if (DirectX::SimpleMath::Vector3::Distance(collider->GetPos(), center_pos) > m_radius) {
		DirectX::SimpleMath::Vector3 direction = collider->GetPos() - center_pos;
		direction.Normalize();
		DirectX::SimpleMath::Vector3 hit_pos = direction * m_radius + center_pos;
		// �Փ˓_���v���C���[�ɓn���ď�����������
		player.HitField(hit_pos);
		// �Փ˃G�t�F�N�g�𔭐�������
		CreateEffect(&player, hit_pos);
	}
}

/// <summary>
/// �t�B�[���h�Փ˃G�t�F�N�g�𔭐�������
/// </summary>
/// <param name="pPlayer">�v���C���[�ւ̃|�C���^</param>
/// <param name="pos">�G�t�F�N�g�𔭐���������W</param>
void Field::CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos) {
	// ����v���C���[����������������Ȃ珈�����Ȃ�
	if (m_effectTimer.count(pPlayer) && m_effectTimer[pPlayer] > 0.0f) {
		return;
	}
	// �n�߂Ĕ���������v���C���[�Ȃ�o�^����
	if (!m_effectTimer.count(pPlayer)) {
		m_effectTimer.emplace(pPlayer, 0.0f);
	}
	// �G�t�F�N�g�𔭐������ă^�C�}�[���Z�b�g����
	m_pEffect->AddEffect(pos);
	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;
	float interval = parameter.lifeTime / parameter.particleNum *2.0f;
	m_effectTimer[pPlayer] = interval;
}

void Field::DrawTurn(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11Device* device = ServiceLocater<DirectX11>::Get()->GetDevice().Get();
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	// ���_���
	DirectX::VertexPositionTexture vertex[4] =
	{
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(0.5f, 0.5f, 0.0f),   DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(-0.5f, 0.5f, 0.0f),  DirectX::SimpleMath::Vector2(1.0f, 0.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(-0.5f, -0.5f, 0.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f)),
		DirectX::VertexPositionTexture(DirectX::SimpleMath::Vector3(0.5f,-0.5f, 0.0f),   DirectX::SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	device->CreateBlendState(&bddesc, &blendstate);

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetReferenceAlpha(0);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(view);
	m_batchEffect->SetProjection(proj);
	m_batchEffect->SetTexture(m_turnTexture.Get());
	//m_batchEffect->SetAlpha(1.0f);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

