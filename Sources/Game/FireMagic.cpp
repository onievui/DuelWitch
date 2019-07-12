#include "FireMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FireMagic::FireMagic()
	: m_object()
	, m_playerId()
	, m_transform()
	, m_vel()
	, m_sphereCollider(&m_transform, FIRE_MAGIC_RADIUS)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FireMagic::~FireMagic() {
}

/// <summary>
/// �����@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FireMagic::Update(const DX::StepTimer& timer) {
	m_lifeTime -= float(timer.GetElapsedSeconds());
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �����@���J������
/// </summary>
void FireMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// �����@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void FireMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), FIRE_MAGIC_RADIUS);
	m_lifeTime = 10.0f;
}

/// <summary>
/// �����@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FireMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �����@�̍s����擾����
/// </summary>
/// <returns>
/// �s��
/// </returns>
const DirectX::SimpleMath::Matrix& FireMagic::GetMatrix() const {
	return m_world;
}

/// <summary>
/// �����@�̓����蔻����擾����
/// </summary>
/// <returns>
/// �����蔻��
/// </returns>
const SphereCollider* FireMagic::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// �v���C���[ID���擾����
/// </summary>
/// <returns>
/// �v���C���[ID
/// </returns>
PlayerID FireMagic::GetPlayerID() const {
	return m_playerId;
}

/// <summary>
/// �����@���g�p���Ă��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : �g�p���Ă���
/// false : �g�p���Ă��Ȃ�
/// </returns>
bool FireMagic::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// �����@���g�p���邩�ǂ����ݒ肷��
/// </summary>
/// <param name="isUsed">true : �g�p����, false : �g�p���Ȃ�</param>
void FireMagic::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}
