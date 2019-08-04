#include "ElementManager.h"
#include <Framework\RandMt.h>
#include "Element.h"
#include "ElementFactory.h"


/// <summary>
/// コンストラクタ
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
ElementManager::~ElementManager() {
}

/// <summary>
/// エレメントマネージャを初期化する
/// </summary>
void ElementManager::Initialize() {
	m_elements.clear();
	m_elements.resize(ElementFactory::ELEMENT_MAX_NUM, nullptr);
	m_elementFactory = std::make_unique<ElementFactory>();
	m_elementFactory->Initialize();
}

/// <summary>
/// エレメントマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ElementManager::Update(const DX::StepTimer& timer) {
	for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		if (*itr) {
			(*itr)->Update(timer);
			if (!(*itr)->IsUsed()) {
				*itr = nullptr;
			}
		}
	}
}

/// <summary>
/// エレメントを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void ElementManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<Element*>::const_iterator itr = m_elements.cbegin(); itr != m_elements.cend(); ++itr) {
		if (*itr) {
			(*itr)->Render(view, proj);
		}
	}
}

/// <summary>
/// エレメントを生成する
/// </summary>
/// <param name="areaStart">生成範囲の始点</param>
/// <param name="areaEnd">生成範囲の終点</param>
/// <param name="num">生成個数</param>
void ElementManager::CreateElement(const DirectX::SimpleMath::Vector3& areaStart, const DirectX::SimpleMath::Vector3& areaEnd, const int num) {
	int rand = RandMt::GetRand(3);
	for (int i = 0; i < num; ++i) {
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
			areaStart.x + (areaEnd.x - areaStart.x)*RandMt::GetRand(1.0f),
			areaStart.y + (areaEnd.y - areaStart.y)*RandMt::GetRand(1.0f),
			areaStart.z + (areaEnd.z - areaStart.z)*RandMt::GetRand(1.0f)
		);
		Element* created_element = m_elementFactory->Create(ElementID(rand), pos);
		for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
			if (!*itr) {
				*itr = created_element;
				break;
			}
		}
		rand = (rand + 1) % 3;
	}
}

/// <summary>
/// エレメントを取得する
/// </summary>
/// <returns>
/// エレメントの配列
/// </returns>
std::vector<Element*>* ElementManager::GetElements() {
	return &m_elements;
}
