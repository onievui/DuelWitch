#include "ThunderMagic.h"
#include <Framework/DirectX11.h>
#include "MagicFactory.h"
#include "MagicManager.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="magicManager">���@�}�l�[�W��</param>
ThunderMagic::ThunderMagic(MagicManager* magicManager)
	: Magic(MagicID::Thunder)
	, m_object()
	, m_pMagicManager(magicManager)
	, m_startTimer() 
	, m_time() {
	m_sphereCollider.SetRadius(THUNDER_MAGIC_COLLIDER_RADIUS);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThunderMagic::~ThunderMagic() {
}

/// <summary>
/// �����@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void ThunderMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = float(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	// ���������p�^�C�}�[�̃J�E���g
	if (m_startTimer) {
		m_time += elapsed_time;
		if (m_time > 0.5f) {
			m_isUsed = false;
		}
	}
	
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;	
	m_transform.SetPosition(pos);

	// ���ŁE�^�C�}�[�̃J�E���g���B�����痎�����@�𐶐�����
	if (!m_isUsed) {
		m_pMagicManager->CreateMagic(MagicID::ThunderStrike, m_playerId, pos, DirectX::SimpleMath::Vector3::Down);
	}

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@���J������
/// </summary>
void ThunderMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void ThunderMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), THUNDER_MAGIC_RADIUS);
	m_lifeTime = 5.0f;
	m_startTimer = false;
	m_time = 0.0f;
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void ThunderMagic::HitPlayer(const SphereCollider& collider) {
	DirectX::SimpleMath::Vector3 direction = collider.GetTransform()->GetPosition() - m_transform.GetPosition();
	// ���Ε����Ȃ珈�����Ȃ�
	if (direction.z*m_vel.z < 0) {
		return;
	}
	// ��苗���܂ŋ߂Â����瑦������
	if (DirectX::SimpleMath::Vector2(direction.x, direction.z).Length() < 3.0f) {
		m_isUsed = false;
	}
	direction.Normalize();
	m_vel = Math::Lerp(m_vel, direction * THUNDER_MAGIC_CHASE_SPEED, 0.5f);
	// �G�v���C���[�ɐڋ߂�����^�C�}�[�쓮
	m_startTimer = true;
}

/// <summary>
/// ���@�Ƃ̏Փˏ���
/// </summary>
/// <param name="other">�Փ˂������@</param>
void ThunderMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	other_id;
}

