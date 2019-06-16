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
	int index = 0;
	for (auto& element : m_elements) {
		if (!element->IsUsed()) {
			break;
		}
		++index;
	}

	if (index == ELEMENT_MAX_NUM) {
		return nullptr;
	}

	switch (id) {
	case ElementID::Fire:
		m_elements[index]->Create(position, DirectX::SimpleMath::Vector4(DirectX::Colors::Red));
		break;
	case ElementID::Thunder:
		m_elements[index]->Create(position, DirectX::SimpleMath::Vector4(DirectX::Colors::Yellow));
		break;
	case ElementID::Freeze:
		m_elements[index]->Create(position, DirectX::SimpleMath::Vector4(DirectX::Colors::SkyBlue));
		break;
	default:
		return nullptr;
	}

	m_elements[index]->IsUsed(true);

	return m_elements[index].get();
}

