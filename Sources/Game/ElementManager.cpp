#include "ElementManager.h"
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\ElementParameter.h>
#include "Element.h"
#include "ElementFactory.h"
#include "PlayParameterLoader.h"
#include "FieldData.h"


/// <summary>
/// コンストラクタ
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory()
	, m_creationTimer() {
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
	m_elements.resize(ElementFactory::GetMaxElementNum(), nullptr);
	m_elementFactory = std::make_unique<ElementFactory>();
	m_elementFactory->Initialize();
	m_creationTimer = 0.0f;

	// フィールド情報にエレメントを登録する
	ServiceLocater<FieldData>::Get()->pElements = &m_elements;
}

/// <summary>
/// エレメントマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ElementManager::Update(const DX::StepTimer& timer) {
	m_creationTimer += static_cast<float>(timer.GetElapsedSeconds());

	// フィールド情報を取得する
	const FieldData* fied_data = ServiceLocater<FieldData>::Get();

	// エレメントを更新する
	for (std::vector<Element*>::iterator itr = LamdaUtils::FindIf(m_elements, LamdaUtils::NotNull());
		itr != m_elements.end();
		LamdaUtils::FindIfNext(itr, m_elements.end(), LamdaUtils::NotNull())) {
		(*itr)->Update(timer);
		(*itr)->FitField(fied_data->fieldCenter, fied_data->fieldRadius);
		if (!(*itr)->IsUsed()) {
			*itr = nullptr;
		}
	}

	// エレメントを生成する
	m_creationTimer = m_creationTimer;
	const float creation_interval = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->creationInterval;
	if (m_creationTimer >= creation_interval) {
		//DirectX::SimpleMath::Vector3 area_offset(0, 0, -60);
		//DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3(-3, -3, -2);
		//DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3(3, 3, 2);
		//CreateElement(area_start + area_offset, area_end + area_offset, 3);
		//area_offset.z = 60.0f;
		//CreateElement(area_start + area_offset, area_end + area_offset, 3);
		
		CreateElement(m_radius, 2, 3);
		m_creationTimer -= creation_interval;
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

///// <summary>
///// エレメントを生成する
///// </summary>
///// <param name="areaStart">生成範囲の始点</param>
///// <param name="areaEnd">生成範囲の終点</param>
///// <param name="num">生成個数</param>
//void ElementManager::CreateElement(const DirectX::SimpleMath::Vector3& areaStart, const DirectX::SimpleMath::Vector3& areaEnd, const int num) {
//	int rand = RandMt::GetRand(3);
//	for (int i = 0; i < num; ++i) {
//		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
//			Math::Lerp(areaStart.x, areaEnd.x, RandMt::GetRand(1.0f)),
//			Math::Lerp(areaStart.y, areaEnd.y, RandMt::GetRand(1.0f)),
//			Math::Lerp(areaStart.z, areaEnd.z, RandMt::GetRand(1.0f))
//		);
//		Element* created_element = m_elementFactory->Create(ElementID(rand), pos);
//		// 未使用のオブジェクトを探す
//		std::vector<Element*>::iterator itr = LamdaUtils::FindIf(m_elements, LamdaUtils::IsNull());
//		if (itr != m_elements.end()) {
//			*itr = created_element;
//		}
//
//		rand = (rand + 1) % 3;
//	}
//}

/// <summary>
/// エレメントを生成する
/// </summary>
/// <param name="radius">エリア半径</param>
/// <param name="groupNum">エレメントグループ数</param>
/// <param name="num">グループ毎の個数</param>
void ElementManager::CreateElement(float radius, int groupNum, int num) {
	// ランダムに方向を決める（横360°縦+-30°）
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3::UnitZ;
	dir = DirectX::SimpleMath::Vector3::Transform(dir,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitX, RandMt::GetRange(-Math::PI / 6, Math::PI / 6)));
	dir = DirectX::SimpleMath::Vector3::Transform(dir,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, RandMt::GetRand(Math::PI2)));
	//float angle = RandMt::GetRand(Math::PI2);
	//DirectX::SimpleMath::Vector3 dir(std::cosf(angle), RandMt::GetRange(-1.0f,1.0f)/std::sqrtf(3), std::sinf(angle));
	//dir.Normalize();

	// 回転軸を生成する
	DirectX::SimpleMath::Vector3 xz_dir(dir.x, 0.0f, dir.z);
	DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitY,
		Math::CreateQuaternionFromVector3(xz_dir, dir));
	// 回転を作成する
	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(axis, Math::PI2 / groupNum);
	
	// エレメントを生成する
	for (int i = 0; i < groupNum; ++i) {
		// エレメントをランダムに決める
		int rand = RandMt::GetRand(static_cast<int>(ElementID::Num));
		for (int j = 0; j < num; j++) {
			// 出現位置をランダムで決める
			DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(
				dir.x*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f),
				dir.y*(radius-10.0f)+RandMt::GetRange(-2.0f,2.0f),
				dir.z*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f)
			);
			Element* created_element = m_elementFactory->Create(static_cast<ElementID>(rand), pos);
			// 未使用のオブジェクトを探す
			std::vector<Element*>::iterator itr = LamdaUtils::FindIf(m_elements, LamdaUtils::IsNull());
			if (itr != m_elements.end()) {
				*itr = created_element;
			}
			// エレメントを変更する
			rand = (rand + 1) % static_cast<int>(ElementID::Num);
		}
		dir = DirectX::SimpleMath::Vector3::Transform(dir, quaternion);
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
