#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic()
	, m_pPlayerPos() {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);
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
	m_color = DirectX::Colors::SkyBlue + DirectX::SimpleMath::Color(0, 0, 0, -0.8f);
	m_vel = dir;
	m_lifeTime = parameter.lifeTime;
}

/// <summary>
/// �X���@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = *m_pPlayerPos;
	//pos += m_vel;
	m_transform.SetPosition(pos);

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
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
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
