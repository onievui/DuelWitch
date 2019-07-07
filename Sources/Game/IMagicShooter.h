#pragma once
#ifndef IMAGIC_SHOOTER_DEFINED
#define IMAGIC_SHOOTER_DEFINED


class MagicFactory;
enum class PlayerID;

/// <summary>
/// 魔法発射クラス
/// </summary>
class IMagicShooter {
public:
	virtual ~IMagicShooter() = default;

public:
	// 魔法を更新する
	virtual void Create(MagicFactory* magicFactory, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector3& dir) = 0;

};


#endif // !IMAGIC_SHOOTER_DEFINED


