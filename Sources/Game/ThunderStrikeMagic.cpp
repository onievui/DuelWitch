#include "ThunderStrikeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ThunderStrikeMagic::ThunderStrikeMagic()
	: Magic(MagicID::ThunderStrike) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_sphereCollider.SetRadius(parameter->thunderStrikeParam.radius);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0,-parameter->thunderStrikeParam.height*0.25f,0));

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThunderStrikeMagic::~ThunderStrikeMagic() {
}

/// <summary>
/// �������@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ThunderStrikeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	pos += m_vel * elapsed_time;;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �������@���J������
/// </summary>
void ThunderStrikeMagic::Lost() {

}

/// <summary>
/// �������@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <param name="color">�F</param>
void ThunderStrikeMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
	const DirectX::SimpleMath::Vector4& color) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_sphereCollider.SetRadius(parameter->thunderStrikeParam.radius);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0, -parameter->thunderStrikeParam.height*0.25f, 0));
	m_color = color;
	m_vel = dir*parameter->thunderStrikeParam.moveSpeed;
	m_lifeTime = parameter->thunderStrikeParam.lifeTime;
}

/// <summary>
/// �������@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderStrikeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::ThunderStrikeMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void ThunderStrikeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void ThunderStrikeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// �X���@�ƏՓ˂����璵�˕Ԃ�
	if (other_id == MagicID::Freeze) {
		DirectX::SimpleMath::Vector3 direction = m_transform.GetLocalPosition() - other->GetCollider()->GetPos();
		direction.y *= 0.0f;
		direction.Normalize();

		m_transform.SetRotation(Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::Up, direction));

		m_vel = direction * m_vel.Length();
		//���˕Ԃ��͑���̖��@�����ɂȂ�
		m_playerId = other->GetPlayerID();
	}
}
