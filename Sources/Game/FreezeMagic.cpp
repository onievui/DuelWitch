#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic()
	, m_object()
	, m_pPlayerPos() {
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
/// �v���C���[�Ƃ̏Փˏ���
/// </summary>
/// <param name="collider">�v���C���[�̓����蔻��</param>
void FreezeMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}
