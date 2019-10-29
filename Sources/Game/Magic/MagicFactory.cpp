#include "MagicFactory.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include "IMagic.h"
#include "NormalMagic.h"
#include "FireMagic.h"
#include "ThunderMagic.h"
#include "ThunderStrikeMagic.h"
#include "FreezeMagic.h"
#include <Game\Player\Player.h>


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
/// <param name="magicManager">���@�}�l�[�W��</param>
void MagicFactory::Initialize(MagicManager* magicManager) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	if (static_cast<int>(MagicID::Num) != parameter->maxNum.size()) {
		ErrorMessage(L"���[�h�������@�̎�ސ����s���Ȓl�ł�");
	}
	m_maxNum = parameter->maxNum;
	m_beginIndex.resize(m_maxNum.size());
	m_beginIndex[0] = 0;
	for (unsigned int i = 1; i < m_beginIndex.size(); ++i) {
		m_beginIndex[i] = m_beginIndex[i - 1] + m_maxNum[i - 1];
	}

	m_magics.clear();
	m_magics.resize(GetMagicMaxNum());

	InitializeMagic<NormalMagic>(MagicID::Normal);
	InitializeMagic<FireMagic>(MagicID::Fire);
	InitializeMagic<ThunderMagic>(MagicID::Thunder, magicManager);
	InitializeMagic<ThunderStrikeMagic>(MagicID::ThunderStrike);
	InitializeMagic<FreezeMagic>(MagicID::Freeze);
}

/// <summary>
/// ���@�̐�������
/// </summary>
/// <param name="magicInfo">���@�Ɋւ�����</param>
/// <param name="pos">���W</param>
/// <param name="dir">����</param>
/// <returns>
/// ���@
/// </returns>
IMagic* MagicFactory::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// �g�p���Ă��Ȃ��I�u�W�F�N�g��T��
	std::vector<std::unique_ptr<IMagic>>::iterator begin = m_magics.begin() + m_beginIndex[static_cast<int>(magicInfo.id)];
	std::vector<std::unique_ptr<IMagic>>::iterator end = begin + m_maxNum[static_cast<int>(magicInfo.id)];
	std::vector<std::unique_ptr<IMagic>>::iterator itr = std::find_if_not(begin, end, LamdaUtils::GetLamda(&IMagic::IsUsed));

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == end) {
		return nullptr;
	}

	// ���@�𐶐�����
	(*itr)->Create(magicInfo, pos, dir);
	(*itr)->SetUsed(true);

	return itr->get();
}

/// <summary>
/// �S���@�̍ő�o�������擾����
/// </summary>
/// <returns>
/// �S���@�̍ő�o����
/// </returns>
int MagicFactory::GetMagicMaxNum() {
	int total = 0;
	for (int i = 0; i < static_cast<int>(MagicID::Num); ++i) {
		total += m_maxNum[i];
	}	
	return total;
}
