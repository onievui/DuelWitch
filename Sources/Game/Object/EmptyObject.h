#pragma once
#ifndef EMPTY_OBJECT_DEFINED
#define EMPTY_OBJECT_DEFINED


#include "IObject.h"
#include "Transform.h"


/// <summary>
/// 空のオブジェクトクラス
/// </summary>
class EmptyObject : public IObject{
public:
	// コンストラクタ
	EmptyObject();
	// デストラクタ
	~EmptyObject();

public:
	// オブジェクトを更新する
	void Update(const DX::StepTimer& timer) override;
	// オブジェクトを描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override { view, proj; };

public:
	// オブジェクトの行列を取得する
	const DirectX::SimpleMath::Matrix& GetMatrix() const override   { return m_world; }
	// オブジェクトの当たり判定を取得する
	const Collider*                    GetCollider() const override { return m_collider.get(); }

public:
	// オブジェクトの行列の参照を取得する
	DirectX::SimpleMath::Matrix& GetMatrixRef()    { return m_world; }
	// オブジェクトの姿勢の参照を取得する
	Transform&                   GetTransformRef() { return m_transform; }
	// オブジェクトの当たり判定の参照を取得する
	std::unique_ptr<Collider>&   GetColliderRef()  { return m_collider; }

private:
	// 姿勢
	Transform                   m_transform;
	// 当たり判定
	std::unique_ptr<Collider>   m_collider;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
};


#endif // !EMPTY_OBJECT_DEFINED
