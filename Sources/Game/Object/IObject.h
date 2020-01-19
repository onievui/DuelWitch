#pragma once
#ifndef IOBJECT_DEFINED
#define IOBJECT_DEFINED


#include <Framework\StepTimer.h>


class Collider;


/// <summary>
/// オブジェクトクラス
/// </summary>
class IObject {
public:
	virtual ~IObject() = default;

public:
	// オブジェクトを更新する
	virtual void Update(const DX::StepTimer& timer) = 0;
	// オブジェクトを描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const = 0;

public:
	// オブジェクトの行列を取得する
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() const = 0;
	// オブジェクトの当たり判定を取得する
	virtual const Collider* GetCollider() const = 0;
};


#endif // !IOBJECT_DEFINED


