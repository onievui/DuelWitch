#pragma once
#ifndef ELEMENT_FACTORY_DEFINED
#define ELEMENT_FACTORY_DEFINED

class Element;


// エレメントのID
enum class ElementID {
	Fire,       // 炎エレメント
	Thunder,	// 雷エレメント
	Freeze,     // 氷エレメント
};


/// <summary>
/// エレメントファクトリクラス
/// </summary>
class ElementFactory {
public:
	// エレメントの最大出現数
	static constexpr int ELEMENT_MAX_NUM = 30;

public:
	// コンストラクタ
	ElementFactory();
	// デストラクタ
	~ElementFactory();

public:
	// エレメントファクトリを初期化する
	void Initialize();
	// エレメントを生成する
	Element* Create(ElementID id, const DirectX::SimpleMath::Vector3& position);

private:
	// 管理するエレメント
	std::vector<std::unique_ptr<Element>> m_elements;

};


#endif // !ELEMENT_FACTORY_DEFINED
