#include "Field.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\EffectParameter.h>
#include <Parameters\FieldParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Player\Player.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectID.h>
#include <Game\Effect\FieldShieldEffectEmitter.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field() {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	// ���f���̃G�t�F�N�g��ݒ肷��
	const ModelResource* skydome = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Skydome);
	skydome->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
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

	// �t�B�[���h�̏���������
	m_time = 0.0f;
	m_radius = parameter->defaultScale;
	m_transform.SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));

	// �t�B�[���h����ݒ肵�ăT�[�r�X���P�[�^�ɓo�^����
	m_fieldData.fieldCenter = m_transform.GetPosition();
	m_fieldData.fieldRadius = m_radius;
	m_fieldData.pElements = nullptr;
	ServiceLocater<FieldData>::Register(&m_fieldData);

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
	// �t�B�[���h�����T�[�r�X���P�[�^�����������
	ServiceLocater<FieldData>::Unregister();
}

/// <summary>
/// �t�B�[���h���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Field::Update(const DX::StepTimer& timer) {
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// �G�t�F�N�g�p�^�C�}�[���X�V����
	for (std::map<const Player*, float>::iterator itr = m_effectTimer.begin(); itr != m_effectTimer.end(); ++itr) {
		itr->second -= elapsed_time;
	}

	// ��莞�Ԍo�ߌ�A�t�B�[���h�����X�ɏ���������
	if (m_time > parameter->startScaleDownTime) {
		m_radius = std::max(m_radius - elapsed_time * parameter->scaleDownSpeed, parameter->minScale);
		m_fieldData.fieldRadius = m_radius;
	}
}

/// <summary>
/// �t�B�[���h��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void Field::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	DirectX::SimpleMath::Vector3 center_pos = m_transform.GetLocalPosition();

	// �t�B�[���h�̍s����쐬����
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_radius / parameter->defaultScale);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);

	// �X�J�C�h�[����`�悷��
	ModelResource* skydome = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetRawResource(ModelID::Skydome);
	skydome->GetResource()->Draw(context, *states, DirectX::SimpleMath::Matrix::CreateScale(1)*world, view, proj);

	// �O�ǂ�`�悷��
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
