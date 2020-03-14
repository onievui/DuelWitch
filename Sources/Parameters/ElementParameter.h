#pragma once
#ifndef ELEMENT_PARAMETER_DEFINED
#define ELEMENT_PARAMETER_DEFINED


#include <Utils\ILoadData.h>


/// <summary>
/// エレメント用パラメータ
/// </summary>
class ElementParameter : public ILoadData {
public:
	// パラメータを読み込む
	bool Load() override;
	// パラメータを再読み込みする
	bool Reload() override;
	// パラメータを保存する
	bool Save() override;

private:
	// パラメータを読み込む
	bool LoadData();

public:
	int maxNum;	                             // エレメントの最大出現数
	float alphaRate;	                     // エレメントの不透明度
	float radius;                            // エレメントの半径
	float creationInterval;                  // エレメントの生成間隔
	DirectX::SimpleMath::Vector3 appearArea; // エレメントのランダム出現範囲
	float minDistance;                       // 同時に生成するエレメントの最小間隔

};


#endif // !ELEMENT_PARAMETER_DEFINED
