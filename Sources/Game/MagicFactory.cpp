#include "MagicFactory.h"
#include "IMagic.h"
#include "NormalMagic.h"


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
	m_magics.resize(MAGIC_MAX_NUM);
	for (auto& magic : m_magics) {
		magic = std::make_unique<NormalMagic>();
	}
}

/// <summary>
/// ���@�̐�������
/// </summary>
/// <param name="id">���@ID</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <returns>
/// ���@
/// </returns>
IMagic* MagicFactory::Create(MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	int index = 0;
	for (auto& magic : m_magics) {
		if (!magic->IsUsed()) {
			break;
		}
		++index;
	}

	if (index == MAGIC_MAX_NUM) {
		return nullptr;
	}

	switch (id) {
	case MagicID::Normal:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::White));
		break;
	case MagicID::Fire:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case MagicID::Thunder:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case MagicID::Freeze:
		m_magics[index]->Create(pos, dir, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
		break;
	default:
		return nullptr;
	}

	m_magics[index]->IsUsed(true);

	return m_magics[index].get();
}

