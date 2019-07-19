#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


class IMagic;
enum class PlayerID;


/// <summary>
/// 魔法ファクトリクラス
/// </summary>
class MagicFactory {
public:	
	// 魔法のID
	enum class MagicID {
		Normal,		// 通常魔法
		Fire,       // ファイアの魔法
		Thunder,	// サンダーの魔法
		Freeze,     // フリーズの魔法
	};

private:
	// 各魔法の最大出現数
	static constexpr int MAGIC_NUM[] = {
		30,
		30,
		10,
		10,
	};

	// 配列のインデックス初期位置
	static constexpr int MAGIC_BEGIN_INDEX[] = {
		0,
		30,
		60,
		70,
	};

public:
	// コンストラクタ
	MagicFactory();
	~MagicFactory();

public:
	// 魔法ファクトリを初期化する
	void Initialize();
	// 魔法を生成する
	IMagic* Create(MagicID id, PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// 全魔法の最大出現数を取得する
	static int GetMagicMaxNum();
	
private:
	template <class T>
	// 各魔法を初期化する
	void InitializeMagic(MagicID id);

private:
	// 管理する魔法
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


#endif // !MAGIC_FACTORY_DEFINED


/// <summary>
/// 魔法クラス
/// </summary>
template<class T>
/// <summary>
/// 各魔法を初期化する
/// </summary>
/// <param name="id">魔法のID</param>
inline void MagicFactory::InitializeMagic(MagicID id) {
	for (auto itr = m_magics.begin() + MAGIC_BEGIN_INDEX[(int)id], end = itr + MAGIC_NUM[(int)id];
		itr != end; ++itr) {
		*itr = std::make_unique<T>();
	}
}
