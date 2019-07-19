#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FreezeMagic::FreezeMagic()
	: m_object()
	, m_playerId()
	, m_transform()
	, m_vel()
	, m_sphereCollider(&m_transform, FREEZE_MAGIC_RADIUS)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// �X���@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	m_lifeTime -= float(timer.GetElapsedSeconds());
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
	m_object.reset();
}

/// <summary>
/// �X���@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void FreezeMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_pPlayerPos = &pos;
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), FREEZE_MAGIC_RADIUS);
	m_lifeTime = 6.0f;
}

/// <summary>
/// �X���@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �X���@�̍s����擾����
/// </summary>
/// <returns>
/// �s��
/// </returns>
const DirectX::SimpleMath::Matrix& FreezeMagic::GetMatrix() const {
	return m_world;
}

/// <summary>
/// �X���@�̓����蔻����擾����
/// </summary>
/// <returns>
/// �����蔻��
/// </returns>
const SphereCollider* FreezeMagic::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// �v���C���[ID���擾����
/// </summary>
/// <returns>
/// �v���C���[ID
/// </returns>
PlayerID FreezeMagic::GetPlayerID() const {
	return m_playerId;
}

/// <summary>
/// �X���@���g�p���Ă��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : �g�p���Ă���
/// false : �g�p���Ă��Ȃ�
/// </returns>
bool FreezeMagic::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// �X���@���g�p���邩�ǂ����ݒ肷��
/// </summary>
/// <param name="isUsed">true : �g�p����, false : �g�p���Ȃ�</param>
void FreezeMagic::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}
