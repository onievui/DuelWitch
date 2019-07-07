#include "NormalMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
NormalMagic::NormalMagic()
	: m_object()
	, m_playerId()
	, m_transform()
	, m_vel()
	, m_sphereCollider(&m_transform, NORMAL_MAGIC_RADIUS)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
NormalMagic::~NormalMagic() {
}

/// <summary>
/// �ʏ햂�@���X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void NormalMagic::Update(const DX::StepTimer& timer) {
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
/// �ʏ햂�@���J������
/// </summary>
void NormalMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// �ʏ햂�@�𐶐�����
/// </summary>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="vel">���x</param>
/// <param name="color">�F</param>
void NormalMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), NORMAL_MAGIC_RADIUS);
	m_lifeTime = 10.0f;
}

/// <summary>
/// �ʏ햂�@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void NormalMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// �ʏ햂�@�̍s����擾����
/// </summary>
/// <returns>
/// �s��
/// </returns>
const DirectX::SimpleMath::Matrix& NormalMagic::GetMatrix() const {
	return m_world;
}

/// <summary>
/// �ʏ햂�@�̓����蔻����擾����
/// </summary>
/// <returns>
/// �����蔻��
/// </returns>
const SphereCollider* NormalMagic::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// �v���C���[ID���擾����
/// </summary>
/// <returns>
/// �v���C���[ID
/// </returns>
PlayerID NormalMagic::GetPlayerID() const {
	return m_playerId;
}

/// <summary>
/// �ʏ햂�@���g�p���Ă��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : �g�p���Ă���
/// false : �g�p���Ă��Ȃ�
/// </returns>
bool NormalMagic::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// �ʏ햂�@���g�p���邩�ǂ����ݒ肷��
/// </summary>
/// <param name="isUsed">true : �g�p����, false : �g�p���Ȃ�</param>
void NormalMagic::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}
