#include "ElementFactory.h"
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
	m_elements.resize(ELEMENT_MAX_NUM);
	for (auto& element : m_elements) {
		element = std::make_unique<Element>();
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
	auto itr = std::find_if(m_elements.begin(), m_elements.end(), [](auto& element) {return !element->IsUsed(); });

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	switch (id) {
	case ElementID::Fire:
		(*itr)->Create(id, position, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case ElementID::Thunder:
		(*itr)->Create(id, position, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case ElementID::Freeze:
		(*itr)->Create(id, position, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
		break;
	default:
		return nullptr;
	}

	(*itr)->IsUsed(true);

	return itr->get();
}

