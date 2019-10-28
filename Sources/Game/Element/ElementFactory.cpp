#include "ElementFactory.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "Element.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
ElementFactory::ElementFactory() {
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ElementFactory::~ElementFactory() {
}

/// <summary>
/// �G�������g�t�@�N�g��������������
/// </summary>
void ElementFactory::Initialize() {
	m_elements.clear();
	m_elements.resize(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum);
	for (std::vector<std::unique_ptr<Element>>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		(*itr) = std::make_unique<Element>();
	}
}

/// <summary>
/// �G�������g�̐�������
/// </summary>
/// <param name="id">�G�������gID</param>
/// <param name="position">���W</param>
/// <returns>
/// �G�������g
/// </returns>
Element* ElementFactory::Create(ElementID id, const DirectX::SimpleMath::Vector3& position) {
	// �g�p���Ă��Ȃ��I�u�W�F�N�g��T��
	std::vector<std::unique_ptr<Element>>::iterator itr = LamdaUtils::FindIfNot(m_elements, LamdaUtils::GetLamda(&Element::IsUsed));

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	DirectX::SimpleMath::Color color;
	const float alpha_rate = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->alphaRate;

	switch (id) {
	case ElementID::Fire:
		color = DirectX::Colors::Red;
		color.w *= alpha_rate;
		(*itr)->Create(id, position, color);
		break;
	case ElementID::Thunder:
		color = DirectX::Colors::Yellow;
		color.w *= alpha_rate;
		(*itr)->Create(id, position, color);
		break;
	case ElementID::Freeze:
		color = DirectX::Colors::SkyBlue;
		color.w *= alpha_rate;
		(*itr)->Create(id, position, color);
		break;
	default:
		return nullptr;
	}

	(*itr)->SetUsed(true);

	return itr->get();
}

/// <summary>
/// �����Ɉ�����ő�̃G�������g�����擾����
/// </summary>
/// <returns>
/// �ő吔
/// </returns>
int ElementFactory::GetMaxElementNum() {
	return ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum;
}

