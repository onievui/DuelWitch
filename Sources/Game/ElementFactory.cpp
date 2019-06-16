#include "ElementFactory.h"
#include "Element.h"


/// <summary>
/// コンストラクタ
/// </summary>
ElementFactory::ElementFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
ElementFactory::~ElementFactory() {
}

/// <summary>
/// エレメントファクトリを初期化する
/// </summary>
void ElementFactory::Initialize() {
	m_elements.clear();
	m_elements.resize(ELEMENT_MAX_NUM);
	for (auto& element : m_elements) {
		element = std::make_unique<Element>();
	}
}

/// <summary>
/// エレメントの生成処理
/// </summary>
/// <param name="id">エレメントID</param>
/// <param name="position">座標</param>
/// <returns>
/// エレメント
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

