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
	// 使用していないオブジェクトを探す
	auto itr = std::find_if(m_elements.begin(), m_elements.end(), [](auto& element) {return !element->IsUsed(); });

	// これ以上生成できないならnullptrを返す
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

