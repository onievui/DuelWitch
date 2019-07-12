#include "MagicFactory.h"
#include "IMagic.h"
#include "NormalMagic.h"
#include "FireMagic.h"
#include "Player.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
MagicFactory::MagicFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MagicFactory::~MagicFactory() {
}

/// <summary>
/// ���@�t�@�N�g��������������
/// </summary>
void MagicFactory::Initialize() {
	m_magics.clear();
	m_magics.resize(GetAllMagicMaxNum());
	for (auto itr = m_magics.begin() + MagicBeginIndex[(int)MagicID::Normal], end = itr + MagicMaxNum[(int)MagicID::Normal];
		itr != end; ++itr) {
		*itr = std::make_unique<NormalMagic>();
	}
	for (auto itr = m_magics.begin() + MagicBeginIndex[(int)MagicID::Fire], end = itr + MagicMaxNum[(int)MagicID::Fire];
		itr != end; ++itr) {
		*itr = std::make_unique<FireMagic>();
	}
}

/// <summary>
/// ���@�̐�������
/// </summary>
/// <param name="id">���@ID</param>
/// <param name="playerId">�v���C���[ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <returns>
/// ���@
/// </returns>
IMagic* MagicFactory::Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// �g�p���Ă��Ȃ��I�u�W�F�N�g��T��
	auto begin = m_magics.begin() + MagicBeginIndex[(int)id];
	auto end = begin + MagicMaxNum[(int)id];
	auto itr = std::find_if(begin, end, [](auto& element) {return !element->IsUsed(); });

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == end) {
		return nullptr;
	}

	switch (id) {
	case MagicID::Normal:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::White));
		break;
	case MagicID::Fire:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case MagicID::Thunder:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case MagicID::Freeze:
		(*itr)->Create(playerId, pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
		break;
	default:
		return nullptr;
	}

	(*itr)->IsUsed(true);

	return itr->get();
}

/// <summary>
/// �S���@�̍ő�o�������擾����
/// </summary>
/// <returns>
/// �S���@�̍ő�o����
/// </returns>
int MagicFactory::GetAllMagicMaxNum() {
	int total = 0;
	for (auto& num : MagicMaxNum) {
		total += num;
	}
	return total;
}

