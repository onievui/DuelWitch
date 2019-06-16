#pragma once
#ifndef ELEMENT_FACTORY_DEFINED
#define ELEMENT_FACTORY_DEFINED

class Element;

/// <summary>
/// エレメントファクトリクラス
/// </summary>
class ElementFactory {
public:
	// エレメントの最大出現数
	static constexpr int ELEMENT_MAX_NUM = 30;
	// エレメントのID
	enum class ElementID {
		Fire,       //ファイアエレメント
		Thunder,	//サンダーエレメント
		Freeze,     //フリーズエレメント
	};

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
