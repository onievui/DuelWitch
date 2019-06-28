#pragma once
#ifndef IMAGIC_DEFINED
#define IMAGIC_DEFINED


#include "IObject.h"


/// <summary>
/// 魔法クラス
/// </summary>
class IMagic : public IObject {
public:
	virtual ~IMagic() = default;

public:
	// 魔法を更新する
	virtual void Update(const DX::StepTimer& timer) = 0;
	// 魔法を開放する
	virtual void Lost() = 0;
	// 魔法を生成する
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
		const DirectX::SimpleMath::Vector4& color) = 0;
	// 魔法を描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const = 0;

public:
	// エレメントを使用しているかどうか取得する
	virtual bool IsUsed() const = 0;
	// エレメントを使用するかどうか設定する
	virtual void IsUsed(bool isUsed) = 0;
};


#endif // !IMAGIC_DEFINED


