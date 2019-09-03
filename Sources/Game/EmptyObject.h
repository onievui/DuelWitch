//#pragma once
//#ifndef EMPTY_OBJECT_DEFINED
//#define EMPTY_OBJECT_DEFINED
//
//
//#include "IObject.h"
//
//
//class Transform;
//
//
///// <summary>
///// 空のオブジェクトクラス
///// </summary>
//class EmptyObject : public IObject{
//public:
//	// コンストラクタ
//	EmptyObject() = default;
//	// デストラクタ
//	~EmptyObject() = default;
//
//public:
//	// オブジェクトを更新する
//	void Update(const DX::StepTimer& timer) override { timer; }
//	// オブジェクトを開放する
//	void Lost() override {};
//	// オブジェクトを描画する
//	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const override { view, proj; };
//
//public:
//	// オブジェクトの行列を取得する
//	const DirectX::SimpleMath::Matrix& GetMatrix() const override;
//	// オブジェクトの当たり判定を取得する
//	const SphereCollider* GetCollider() const override;
//
//private:
//	Transform m_trans
//	SphereCollider m_collider;
//};
//
//
//#endif // !EMPTY_OBJECT_DEFINED
