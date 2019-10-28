#include "ElementFactory.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\ElementParameter.h>
#include <Game\Load\PlayParameterLoader.h>
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
	std::vector<std::unique_ptr<Element>>::iterator itr = LamdaUtils::FindIfNot(m_elements, LamdaUtils::GetLamda(&Element::IsUsed));

	// これ以上生成できないならnullptrを返す
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
/// 同時に扱える最大のエレメント数を取得する
/// </summary>
/// <returns>
/// 最大数
/// </returns>
int ElementFactory::GetMaxElementNum() {
	return ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->maxNum;
}

