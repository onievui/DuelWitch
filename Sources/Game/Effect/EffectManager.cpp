#include "EffectManager.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\LamdaUtils.h>
#include <Utils\ErrorMessage.h>
#include "EffectFactory.h"
#include "IEffectEmitter.h"


/// <summary>
///	コンストラクタ
/// </summary>
EffectManager::EffectManager()
	: m_effects()
	, m_effectFactory()
	, m_batch() {
}

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager() {

}

/// <summary>
/// エフェクトマネージャを初期化する
/// </summary>
void EffectManager::Initialize() {
	m_effectFactory = std::make_unique<EffectFactory>();
	m_effectFactory->Initialize(this);
	m_effects.clear();
	m_effects.resize(m_effectFactory->GetEffectMaxNum(), nullptr);
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
}

/// <summary>
/// エフェクトマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
/// <param name="camera">カメラ</param>
void EffectManager::Update(const DX::StepTimer& timer, const Camera* camera) {
	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		if ((*itr)->IsUsed()) {
			(*itr)->Update(timer, camera);
		}
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
	

	for (std::vector<IEffectEmitter*>::iterator itr = LamdaUtils::FindIf(m_effects, LamdaUtils::NotNull());
		itr != m_effects.end();
		LamdaUtils::FindIfNext(itr, m_effects.end(), LamdaUtils::NotNull())) {
		(*itr)->Render(m_batch.get(), view, proj);
	}

	// シェーダを解除する
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
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
