#include "ElementFactory.h"
#include <Utils\ServiceLocater.h>
#include <Parameters\ElementParameter.h>
#include "PlayParameterLoader.h"
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
	m_elements.resize(ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum);
	for (std::vector<std::unique_ptr<Element>>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		(*itr) = std::make_unique<Element>();
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
	std::vector<std::unique_ptr<Element>>::iterator itr = std::find_if(m_elements.begin(), m_elements.end(),
		[](std::unique_ptr<Element>& element) {return !element->IsUsed(); });

	// これ以上生成できないならnullptrを返す
	if (itr == m_elements.end()) {
		return nullptr;
	}
	
	DirectX::SimpleMath::Vector4 color;
	const float& alphaRate = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->alphaRate;

	switch (id) {
	case ElementID::Fire:
		color = DirectX::Colors::Red;
		color.w *= alphaRate;
		(*itr)->Create(id, position, color);
		break;
	case ElementID::Thunder:
		color = DirectX::Colors::Yellow;
		color.w *= alphaRate;
		(*itr)->Create(id, position, color);
		break;
	case ElementID::Freeze:
		color = DirectX::Colors::SkyBlue;
		color.w *= alphaRate;
		(*itr)->Create(id, position, color);
		break;
	default:
		return nullptr;
	}

	(*itr)->IsUsed(true);

	return itr->get();
}

/// <summary>
/// 同時に扱える最大のエレメント数を取得する
/// </summary>
/// <returns>
/// 最大数
/// </returns>
int ElementFactory::GetMaxElementNum() {
	return ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum;
}

