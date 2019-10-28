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
	// エレメント出現エリアの半径を設定する
	void SetRadius(float radius) { m_radius = radius; }
	// エレメントを取得する
	std::vector<Element*>* GetElements();

private:
	// エレメントを生成する
	void CreateElement(float radius, int groupNum, int num);

private:
	// 生成済みエレメント
	std::vector<Element*>           m_elements;
	// エレメントファクトリ
	std::unique_ptr<ElementFactory> m_elementFactory;
	// 生成用タイマー
	float                           m_creationTimer;
	// エレメント出現エリア半径
	float                           m_radius;
};


#endif // !ELEMENT_MANAGER_DEFINED
