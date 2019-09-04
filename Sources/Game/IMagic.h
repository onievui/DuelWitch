#pragma once
#ifndef IMAGIC_DEFINED
#define IMAGIC_DEFINED


#include "IObject.h"


class SphereCollider;
enum class MagicID;
enum class PlayerID;


/// <summary>
/// 魔法インタフェースクラス
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
	virtual void Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
		const DirectX::SimpleMath::Vector4& color) = 0;
	// 魔法を描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const = 0;

public:
	// 魔法IDを取得する
	virtual MagicID GetID() const = 0;
	// プレイヤーIDを取得する
	virtual PlayerID GetPlayerID() const = 0;
	// プレイヤーとの衝突処理
	virtual void HitPlayer(const Collider* collider) = 0;
	// 魔法との衝突処理
	virtual void HitMagic(const IMagic* other) = 0;

	// 魔法を使用しているかどうか取得する
	virtual bool IsUsed() const = 0;
	// 魔法を使用するかどうか設定する
	virtual void IsUsed(bool isUsed) = 0;
};


#endif // !IMAGIC_DEFINED


