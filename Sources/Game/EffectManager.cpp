#include "EffectManager.h"
#include <Utils\LamdaUtils.h>
#include <Utils\ErrorMessage.h>
#include "EffectFactory.h"
#include "IEffectEmitter.h"


/// <summary>
///	コンストラクタ
/// </summary>
EffectManager::EffectManager()
	: m_effects()
	, m_effectFactory() {
}

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager() {

}

/// <summary>
/// エフェクトマネージャを初期する
/// </summary>
void EffectManager::Initialize() {
	m_effectFactory = std::make_unique<EffectFactory>();
	m_effectFactory->Initialize(this);
	m_effects.clear();
	m_effects.resize(m_effectFactory->GetEffectMaxNum(), nullptr);
}

/// <summary>
/// エフェクトマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
/// <param name="camera">カメラ</param>
void EffectManager::Update(const DX::StepTimer& timer, const Camera* camera) {
	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		(*itr)->Update(timer, camera);
		// 未使用なら外す
		if (!(*itr)->IsUsed()) {
			*itr = nullptr;
		}
	}
}

/// <summary>
/// エフェクトマネージャを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void EffectManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		(*itr)->Render(view, proj);
	}
}

/// <summary>
/// エフェクトを生成する
/// </summary>
/// <param name="id"></param>
/// <param name="pos"></param>
/// <param name="dir"></param>
IEffectEmitter* EffectManager::CreateEffect(EffectID id, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf()(m_effects, LamdaUtils::IsNull());
	// 空きがある場合に登録する
	if (itr != m_effects.end()) {
		*itr = m_effectFactory->Create(id, pos, dir);
		return *itr;
	}
	
	ErrorMessage(L"エフェクトの空きがなく、生成に出来ませんでした");
	return nullptr;
}
