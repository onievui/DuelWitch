#pragma once
#ifndef ELEMENT_MANAGER_DEFINED
#define ELEMENT_MANAGER_DEFINED


#include <Framework\StepTimer.h>


class Element;
class ElementFactory;


/// <summary>
/// エレメントマネージャクラス
/// </summary>
class ElementManager {
public:
	// コンストラクタ
	ElementManager();
	// デストラクタ
	~ElementManager();

public:
	// エレメントマネージャを初期化する
	void Initialize();
	// エレメントマネージャを更新する
	void Update(const DX::StepTimer& timer);
	// エレメントを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// エレメントを生成する
	void CreateElement(const DirectX::SimpleMath::Vector3& areaStart, const DirectX::SimpleMath::Vector3& areaEnd, const int num);
	// エレメントを取得する
	std::vector<Element*>* GetElements();

private:
	// 生成済みエレメント
	std::vector<Element*> m_elements;
	// エレメントファクトリ
	std::unique_ptr<ElementFactory> m_elementFactory;

};


#endif // !ELEMENT_MANAGER_DEFINED
