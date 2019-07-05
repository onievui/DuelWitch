#pragma once
#ifndef MAGIC_MANAGER_DEFINED
#define MAGIC_MANAGER_DEFINED


#include <Framework\StepTimer.h>
#include "MagicFactory.h"


class IMagic;
class IMagicShooter;


/// <summary>
/// 魔法マネージャクラス
/// </summary>
class MagicManager {
public:
	// コンストラクタ
	MagicManager();
	// デストラクタ
	~MagicManager();

public:
	// 魔法マネージャを初期化する
	void Initialize();
	// 魔法マネージャを更新する
	void Update(const DX::StepTimer& timer);
	// 魔法を描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	// 魔法を生成する
	void CreateMagic(MagicFactory::MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// 魔法を取得する
	std::vector<IMagic*>* GetMagics();

private:
	// 生成済み魔法
	std::vector<IMagic*>                           m_magics;
	// 魔法ファクトリ
	std::unique_ptr<MagicFactory>                  m_magicFactory;
	// 魔法発射クラス
	std::vector<std::unique_ptr<IMagicShooter>>    m_magicShooters;

};


#endif // !MAGIC_MANAGER_DEFINED
