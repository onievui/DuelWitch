#pragma once
#ifndef EFFECT_FACTORY_DEFINED
#define EFFECT_FACTORY_DEFINED


class IEffectEmitter;
class EffectManager;
enum class EffectID;


/// <summary>
/// エフェクトファクトリクラス
/// </summary>
class EffectFactory {
public:
	// コンストラクタ
	EffectFactory();
	// デストラクタ
	~EffectFactory();

public:
	// エフェクトファクトリを初期化する
	void Initialize(EffectManager* effectManager);
	// エフェクトを生成して返す
	IEffectEmitter* Create(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// 全エフェクトの最大出現数を取得する
	int GetEffectMaxNum();

private:
	template <class T, class... Args>
	// 各エフェクトを初期化する
	void InitializeEffect(EffectID id, Args&&... args);

private:
	// 管理するエフェクト
	std::vector<std::unique_ptr<IEffectEmitter>> m_effects;
	// エフェクトの最大出現数
	std::vector<int>                             m_maxNum;
	// 配列のインデックス初期位置
	std::vector<int>                             m_beginIndex;
};


template<class T, class... Args>
/// <summary>
/// 各エフェクトを初期化する
/// </summary>
/// <param name="id">エフェクトのID</param>
inline void EffectFactory::InitializeEffect(EffectID id, Args&&... args) {
	for (std::vector<std::unique_ptr<IEffectEmitter>>::iterator itr = m_effects.begin() + m_beginIndex[static_cast<int>(id)],
		end = itr + m_maxNum[static_cast<int>(id)]; itr != end; ++itr) {
		*itr = std::make_unique<T>(args...);
	}
}


#endif // !EFFECT_FACTORY_DEFINED

