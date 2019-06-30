#pragma once
#ifndef MAGIC_FACTORY_DEFINED
#define MAGIC_FACTORY_DEFINED

class IMagic;

/// <summary>
/// 魔法ファクトリクラス
/// </summary>
class MagicFactory {
public:
	// 魔法の最大出現数
	static constexpr int MAGIC_MAX_NUM = 30;
	// 魔法のID
	enum class MagicID {
		Normal,		// 通常魔法
		Fire,       // ファイアの魔法
		Thunder,	// サンダーの魔法
		Freeze,     // フリーズの魔法
	};

public:
	// コンストラクタ
	MagicFactory();
	// デストラクタ
	~MagicFactory();

public:
	// 魔法ファクトリを初期化する
	void Initialize();
	// 魔法を生成する
	IMagic* Create(MagicID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);

private:
	// 管理する魔法
	std::vector<std::unique_ptr<IMagic>> m_magics;

};


#endif // !MAGIC_FACTORY_DEFINED
