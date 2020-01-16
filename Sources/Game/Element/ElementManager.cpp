#include "ElementManager.h"
#include <unordered_map>
#include <Utils\ServiceLocater.h>
#include <Utils\RandMt.h>
#include <Utils\MathUtils.h>
#include <Utils\LamdaUtils.h>
#include <Utils\IfIterator.h>
#include <Parameters\ElementParameter.h>
#include "Element.h"
#include "ElementFactory.h"
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Field\FieldData.h>


/// <summary>
/// コンストラクタ
/// </summary>
ElementManager::ElementManager()
	: m_elements()
	, m_elementFactory()
	, m_creationTimer()
	, m_center()
	, m_radius() {
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
	for (IfIterator<std::vector<Element*>> itr(m_elements, LamdaUtils::NotNull()); itr != m_elements.end(); ++itr) {
		(*itr)->Update(timer);
		// フィールドとの位置関係を保つ
		(*itr)->FitField(fied_data->fieldCenter, fied_data->fieldRadius);
		// 未使用のエレメントをnullにする
		if (!(*itr)->IsUsed()) {
			*itr = nullptr;
		}
	}

	// エレメントを生成する
	const float creation_interval = ServiceLocater<PlayParameterLoader>::Get()->GetElementParameter()->creationInterval;
	// インターバルを超えたら生成する
	if (m_creationTimer >= creation_interval) {
		// 2方向に3つずつ生成する
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

	// 有効なエレメントの配列を生成する
	std::list<Element*> valid_elements;
	// ポインタと深度値のマップを生成する
	std::unordered_map<Element*, float> sort_keys;
	for (std::vector<Element*>::iterator itr = m_elements.begin(); itr != m_elements.end(); ++itr) {
		if (*itr) {
			const DirectX::SimpleMath::Vector3& pos = (*itr)->GetPos();
			// 比較用深度値を計算する
			float key = view._13*pos.x + view._23*pos.y + view._33*pos.z;
			// 深度値とポインタを紐づける
			sort_keys.emplace((*itr), key);
			// 有効なエレメントの配列に追加する
			valid_elements.push_back((*itr));
		}
	}

	// Zソートを行う
	valid_elements.sort([&](Element* left, Element* right) { return sort_keys[left] < sort_keys[right]; });

	// 有効なエレメントを描画する
	for (std::list<Element*>::const_iterator itr = valid_elements.cbegin(); itr != valid_elements.cend(); ++itr) {
		(*itr)->Render(view, proj);
	}
}

/// <summary>
/// エレメントを生成する
/// </summary>
/// <param name="radius">フィールド半径</param>
/// <param name="groupNum">エレメントグループ数</param>
/// <param name="num">グループ毎の個数</param>
void ElementManager::CreateElement(float radius, int groupNum, int num) {
	// フィールド情報を取得する
	const FieldData* fied_data = ServiceLocater<FieldData>::Get();

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
			DirectX::SimpleMath::Vector3 pos = m_center + DirectX::SimpleMath::Vector3(
				dir.x*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f),
				dir.y*(radius-10.0f)+RandMt::GetRange(-2.0f,2.0f),
				dir.z*(radius-10.0f)+RandMt::GetRange(-3.0f,3.0f)
			);
			Element* created_element = m_elementFactory->Create(static_cast<ElementID>(rand), pos);
			// 未使用のオブジェクトを探す
			std::vector<Element*>::iterator itr = LamdaUtils::FindIf(m_elements, LamdaUtils::IsNull());
			if (itr != m_elements.end()) {
				*itr = created_element;
				// フィールド端との距離を記憶する
				(*itr)->SetBetweenFieldRadius(fied_data->fieldCenter, fied_data->fieldRadius);
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
