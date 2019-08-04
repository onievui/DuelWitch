#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


class IMagic;
class MagicManager;
enum class PlayerID;


// 魔法のID
enum class MagicID {
	Normal,		    // 通常魔法
	Fire,           // 炎魔法
	Thunder,	    // 雷魔法
	ThunderStrike,	// 落雷魔法
	Freeze,         // 氷魔法

	Num
};


/// <summary>
/// 魔法ファクトリクラス
/// </summary>
class MagicFactory {
private:
	// 各魔法の最大出現数
	static constexpr int MAGIC_NUM[static_cast<int>(MagicID::Num)] = {
		30,
		30,
		10,
		10,
		10,
	};

	// 配列のインデックス初期位置
	static constexpr int MAGIC_BEGIN_INDEX[static_cast<int>(MagicID::Num)] = {
		0,
		30,
		60,
		70,
		80,
	};

public:
	// コンストラクタ
	MagicFactory();
	~MagicFactory();

public:
	// 魔法ファクトリを初期化する
	void Initialize(MagicManager* magicManager);
	// 魔法を生成する
	IMagic* Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// 全魔法の最大出現数を取得する
	static int GetMagicMaxNum();
	
private:
	template <class T, class... Args>
	// 各魔法を初期化する
	void InitializeMagic(MagicID id, Args&&... args);

private:
	// 管理する魔法
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


template<class T, class... Args>
/// <summary>
/// 各魔法を初期化する
/// </summary>
/// <param name="id">魔法のID</param>
inline void MagicFactory::InitializeMagic(MagicID id, Args&&... args) {
	for (std::vector<std::unique_ptr<IMagic>>::iterator itr = m_magics.begin() + MAGIC_BEGIN_INDEX[static_cast<int>(id)],
		end = itr + MAGIC_NUM[static_cast<int>(id)]; itr != end; ++itr) {
		*itr = std::make_unique<T>(args...);
	}
}


#endif // !MAGIC_FACTORY_DEFINED

