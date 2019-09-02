#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED


class IMagic;
class MagicManager;
enum class PlayerID;
enum class MagicID;


/// <summary>
/// 魔法ファクトリクラス
/// </summary>
class MagicFactory {
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
	int GetMagicMaxNum();
	
private:
	template <class T, class... Args>
	// 各魔法を初期化する
	void InitializeMagic(MagicID id, Args&&... args);

private:
	// 管理する魔法
	std::vector<std::unique_ptr<IMagic>> m_magics;
	// 魔法の最大出現数
	std::vector<int>                     m_maxNum;
	// 配列のインデックス初期位置
	std::vector<int>                     m_beginIndex;
};


template<class T, class... Args>
/// <summary>
/// 各魔法を初期化する
/// </summary>
/// <param name="id">魔法のID</param>
inline void MagicFactory::InitializeMagic(MagicID id, Args&&... args) {
	for (std::vector<std::unique_ptr<IMagic>>::iterator itr = m_magics.begin() + m_beginIndex[static_cast<int>(id)],
		end = itr + m_maxNum[static_cast<int>(id)]; itr != end; ++itr) {
		*itr = std::make_unique<T>(args...);
	}
}


#endif // !MAGIC_FACTORY_DEFINED

