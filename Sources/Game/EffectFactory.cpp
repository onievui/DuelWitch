#include "EffectFactory.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "EffectManager.h"
#include "EffectEmitter.h"
#include "EffectID.h"
#include "NormalEffectEmitter.h"
#include "FireEffectEmitter.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectFactory::EffectFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EffectFactory::~EffectFactory() {
}

/// <summary>
/// �G�t�F�N�g�t�@�N�g��������������
/// </summary>
/// <param name="effectManager">�G�t�F�N�g�}�l�[�W��</param>
void EffectFactory::Initialize(EffectManager* effectManager) {
	effectManager;

	const EffectParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter();
	if (static_cast<int>(EffectID::Num) != parameter->maxNum.size()) {
		ErrorMessage(L"���[�h�����G�t�F�N�g�̎�ސ����s���Ȓl�ł�");
	}

	// �e�G�t�F�N�g�̌��𒲂ׂ�
	m_maxNum = parameter->maxNum;
	m_beginIndex.resize(m_maxNum.size());
	m_beginIndex[0] = 0;
	for (unsigned int i = 1; i < m_beginIndex.size(); ++i) {
		m_beginIndex[i] = m_beginIndex[i - 1] + m_maxNum[i - 1];
	}

	m_effects.clear();
	m_effects.resize(GetEffectMaxNum());

	// �G�t�F�N�g������������
	InitializeEffect<NormalMagicEffectEmitter>(EffectID::NormalMagic);
	InitializeEffect<FireMagicEffectEmitter>(EffectID::FireMagic);
}

/// <summary>
/// �G�t�F�N�g�𐶐����ĕԂ�
/// </summary>
/// <param name="id">�G�t�F�N�gID</param>
/// <param name="pos">�o���ʒu</param>
/// <param name="dir">����</param>
/// <returns>
/// �G�t�F�N�g�ւ̃|�C���^
/// </returns>
IEffectEmitter* EffectFactory::Create(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// �g�p���Ă��Ȃ��I�u�W�F�N�g��T��
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator begin = m_effects.begin() + m_beginIndex[static_cast<int>(id)];
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator end = begin + m_maxNum[static_cast<int>(id)];
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator itr = std::find_if(begin, end, [](const auto& effct) {return !effct->IsUsed(); });

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == end) {
		return nullptr;
	}

	switch (id) {
	case EffectID::NormalMagic:
		(*itr)->Create(pos, dir);
		break;
	case EffectID::FireMagic:
		(*itr)->Create(pos, dir);
		break;
	default:
		return nullptr;
	}

	(*itr)->IsUsed(true);

	return itr->get();
}

/// <summary>
/// // �S�G�t�F�N�g�̍ő�o�������擾����
/// </summary>
/// <returns>
/// �S�G�t�F�N�g�̍ő�o����
/// </returns>
int EffectFactory::GetEffectMaxNum() {
	int total = 0;
	for (int i = 0; i < static_cast<int>(EffectID::Num); ++i) {
		total += m_maxNum[i];
	}
	return total;
}