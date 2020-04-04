#include "ThunderStrikeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Collision\CapsuleCollider.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectiveEffectEmitter.h>
#include <Game\Player\PlayerData.h>
#include "MagicID.h"
#include "MagicManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pMagicManager">���@�}�l�[�W���ւ̃|�C���^</param>
ThunderStrikeMagic::ThunderStrikeMagic(MagicManager* pMagicManager)
	: Magic()
	, m_pMagicManager(pMagicManager) {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;
	m_collider = std::make_unique<CapsuleCollider>(&m_transform, parameter.radius,
		DirectX::SimpleMath::Vector3(0, parameter.height*0.5f, 0), DirectX::SimpleMath::Vector3(0, -parameter.height*0.5f, 0));

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThunderStrikeMagic::~ThunderStrikeMagic() {
}

/// <summary>
/// �������@�𐶐�����
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
void ThunderStrikeMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	CapsuleCollider* collider = static_cast<CapsuleCollider*>(m_collider.get());
	collider->SetRadius(parameter.radius);
	collider->SetStartPos(DirectX::SimpleMath::Vector3(0, parameter.height*0.5f, 0));
	collider->SetEndPos(DirectX::SimpleMath::Vector3(0, -parameter.height*0.5f, 0));
	m_color = DirectX::Colors::Yellow;
	m_vel = dir * parameter.moveSpeed;
	m_lifeTime = parameter.lifeTime;

	// ���@�̃G�t�F�N�g�𐶐�����
	m_pEffect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::ThunderStrikeMagic, pos, dir);
	m_pEffect->SetParent(&m_transform);

	m_refrectCount = 0;
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

	// �߂��ɕX���@������΁A�����񂹂���
	std::vector<IMagic*>* magics = m_pMagicManager->GetMagics();
	if (m_refrectCount == 0) {
		for (IfIterator<std::vector<IMagic*>> itr(*magics, LamdaUtils::NotNull()); itr != magics->end(); ++itr) {
			if ((*itr)->GetID() != MagicID::Freeze) {
				continue;
			}
			const DirectX::SimpleMath::Vector3& other_pos = (*itr)->GetTransform().GetLocalPosition();
			DirectX::SimpleMath::Vector3 vec(other_pos.x - pos.x, 0, other_pos.z - pos.z);
			if (vec.Length() <= ATTRACTED_RADIUS && other_pos.y < pos.y) {
				pos += vec * ATTRACTED_SPEED;
			}
		}
	}

	pos += m_vel * elapsed_time;;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// �������@��`�悷��
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void ThunderStrikeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	//const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
	//	->GetResource(GeometricPrimitiveID::ThunderStrikeMagic);
	//resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
	view, proj;
}

/// <summary>
/// �������@�̏I���������s��
/// </summary>
void ThunderStrikeMagic::Finalize() {
	// �G�t�F�N�g������
	m_pEffect->SetUsed(false);
}

/// <summary>
/// �_���[�W���擾����
/// </summary>
/// <returns>
/// �_���[�W��
/// </returns>
float ThunderStrikeMagic::GetPower() const {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;

	return parameter.power*m_info.powerRate;
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
		const PlayerData* player_data = ServiceLocater<PlayerData>::Get();
		// ���̏��L�҂Ɍ����Ē��˕Ԃ�
		DirectX::SimpleMath::Vector3 direction =
			player_data->transforms[static_cast<int>(m_info.playerId)]->GetPosition() - m_transform.GetPosition();
		direction.y = 0.0f;
		direction.Normalize();

		m_transform.SetRotation(Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::Up, direction));

		m_vel = direction * m_vel.Length();
		//���˕Ԃ��͑���̖��@�����ɂȂ�
		m_info.playerId = other->GetPlayerID();
		m_info.powerRate = other->GetInfo().powerRate;
		++m_refrectCount;

		// �ł������E���˃G�t�F�N�g�𐶐�����
		IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::Effective,
			m_transform.GetPosition(), m_vel);
		EffectiveEffectEmitter* effective_effect = dynamic_cast<EffectiveEffectEmitter*>(effect);
		if (effective_effect) {
			effective_effect->SetColorID(m_info.id);
		}
	}
}
