#pragma once
#ifndef IEFFECT_EMITTER_DEFINED
#define IEFFECT_EMITTER_DEFINED


#include <Framework\StepTimer.h>


class Camera;
class Transform;


/// <summary>
/// エフェクトエミッターインタフェースクラス
/// </summary>
class IEffectEmitter {
public:
	// 長いので短縮する
	using Batch = DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>;

public:
	// デストラクタ
	virtual ~IEffectEmitter() = default;

public:
	// エフェクトエミッターを生成する
	virtual void Create(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) = 0;
	// エフェクトエミッターを更新する
	virtual void Update(const DX::StepTimer& timer, const Camera* camera) = 0;
	// エフェクトエミッターを描画する
	virtual void Render(Batch* batch, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;

public:
	// 親を設定する
	virtual void SetParent(const Transform* parent) = 0;
	// エフェクトを使用しているかどうか取得する
	virtual bool IsUsed() const = 0;
	// エフェクトを使用するかどうか設定する
	virtual void SetUsed(bool isUsed) = 0;

};


#endif // !IEFFECT_EMITTER_DEFINED
