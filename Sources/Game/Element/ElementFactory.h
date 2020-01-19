#pragma once
#ifndef ELEMENT_FACTORY_DEFINED
#define ELEMENT_FACTORY_DEFINED


#include <Utils\LoadDataHolder.h>


class Element;


// エレメントのID
enum class ElementID {
	Fire,       // 炎エレメント
	Thunder,	// 雷エレメント
	Freeze,     // 氷エレメント

	Num
};


/// <summary>
/// エレメントファクトリクラス
/// </summary>
class ElementFactory {
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

public:
	// 同時に扱える最大のエレメント数を取得する
	static int GetMaxElementNum();

private:
	// 管理するエレメント
	std::vector<std::unique_ptr<Element>>     m_elements;
	// エレメント用ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect>     m_basicEffect;
	// エレメント用入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};


#endif // !ELEMENT_FACTORY_DEFINED
