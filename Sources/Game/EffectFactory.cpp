#include "EffectFactory.h"
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Parameters\EffectParameter.h>
#include "PlayParameterLoader.h"
#include "EffectManager.h"
#include "EffectEmitter.h"
#include "EffectID.h"
#include "PlayerTrailEffectEmitter.h"
#include "FieldShieldEffectEmitter.h"
#include "NormalMagicEffectEmitter.h"
#include "FireMagicEffectEmitter.h"


/// <summary>
/// コンストラクタ
/// </summary>
EffectFactory::EffectFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
EffectFactory::~EffectFactory() {
}

/// <summary>
/// エフェクトファクトリを初期化する
/// </summary>
/// <param name="effectManager">エフェクトマネージャ</param>
void EffectFactory::Initialize(EffectManager* effectManager) {
	effectManager;

	const EffectParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter();
	if (static_cast<int>(EffectID::Num) != parameter->maxNum.size()) {
		ErrorMessage(L"ロードしたエフェクトの種類数が不正な値です");
	}

	// 各エフェクトの個数を調べる
	m_maxNum = parameter->maxNum;
	m_beginIndex.resize(m_maxNum.size());
	m_beginIndex[0] = 0;
	for (unsigned int i = 1; i < m_beginIndex.size(); ++i) {
		m_beginIndex[i] = m_beginIndex[i - 1] + m_maxNum[i - 1];
	}

	m_effects.clear();
	m_effects.resize(GetEffectMaxNum());

	// エフェクトを初期化する
	InitializeEffect<PlayerTrailEffectEmitter>(EffectID::PlayerTrail);
	InitializeEffect<FieldShieldEffectEmitter>(EffectID::FieldShield);
	InitializeEffect<NormalMagicEffectEmitter>(EffectID::NormalMagic);
	InitializeEffect<FireMagicEffectEmitter>(EffectID::FireMagic);

}

/// <summary>
/// エフェクトを生成して返す
/// </summary>
/// <param name="id">エフェクトID</param>
/// <param name="pos">出現位置</param>
/// <param name="dir">方向</param>
/// <returns>
/// エフェクトへのポインタ
/// </returns>
IEffectEmitter* EffectFactory::Create(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	// 使用していないオブジェクトを探す
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator begin = m_effects.begin() + m_beginIndex[static_cast<int>(id)];
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator end = begin + m_maxNum[static_cast<int>(id)];
	std::vector<std::unique_ptr<IEffectEmitter>>::iterator itr = std::find_if_not(begin, end, LamdaUtils::GetLamda(&IEffectEmitter::IsUsed));

	// これ以上生成できないならnullptrを返す
	if (itr == end) {
		return nullptr;
	}

	(*itr)->Create(pos, dir);
	(*itr)->SetUsed(true);

	return itr->get();
}

/// <summary>
/// // 全エフェクトの最大出現数を取得する
/// </summary>
/// <returns>
/// 全エフェクトの最大出現数
/// </returns>
int EffectFactory::GetEffectMaxNum() {
	int total = 0;
	for (int i = 0; i < static_cast<int>(EffectID::Num); ++i) {
		total += m_maxNum[i];
	}
	return total;
}
