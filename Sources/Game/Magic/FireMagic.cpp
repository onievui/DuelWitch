#include "FireMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\IEffectEmitter.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
FireMagic::FireMagic()
	: Magic() {
	const MagicParameter::fire_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FireMagic::~FireMagic() {
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void FireMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::fire_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.radius);
	m_color = DirectX::Colors::Red;
	m_vel = dir * parameter.moveSpeed;
	m_lifeTime = parameter.lifeTime;

	// �����x�N�g�������ɉ~���̉�]�p�x�����߂�
	m_transform.SetRotation(Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::Up, dir));

	// ���@�̃G�t�F�N�g�𐶐�����
	m_pEffect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FireMagic, pos, dir);
	m_pEffect->SetParent(&m_transform);
}


/// <summary>
/// �����@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FireMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	pos += m_vel*elapsed_time;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FireMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	view, proj;
	// ����ɃG�t�F�N�g��`�悵�Ă���
	//const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
	//	->GetResource(GeometricPrimitiveID::FireMagic);
	//resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj,DirectX::SimpleMath::Color(1,1,1,0.7f));
	
}

/// <summary>
/// �����@�̏I���������s��
/// </summary>
void FireMagic::Finalize() {
	// �G�t�F�N�g���I��������
	m_pEffect->SetUsed(false);
}

/// <summary>
/// �_���[�W���擾����
/// </summary>
/// <returns>
/// �_���[�W��
/// </returns>
float FireMagic::GetPower() const {
	const MagicParameter::fire_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->fireParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void FireMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void FireMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// �������@�ƏՓ˂����������
	if (other_id == MagicID::ThunderStrike) {
		m_isUsed = false;
	}
}
