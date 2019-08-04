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
	std::vector<std::unique_ptr<Element>>::iterator itr = std::find_if(m_elements.begin(), m_elements.end(),
		[](std::unique_ptr<Element>& element) {return !element->IsUsed(); });

	// ����ȏ㐶���ł��Ȃ��Ȃ�nullptr��Ԃ�
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	DirectX::SimpleMath::Vector4 color;
	constexpr float alpha_rate = 0.8f;

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

	(*itr)->IsUsed(true);

	return itr->get();
}

