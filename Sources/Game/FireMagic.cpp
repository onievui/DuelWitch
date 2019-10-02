#include "FireMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "Player.h"
#include "EffectManager.h"
#include "IEffectEmitter.h"


/// <summary>
/// コンストラクタ
/// </summary>
FireMagic::FireMagic()
	: Magic(MagicID::Fire) {
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_sphereCollider.SetRadius(parameter->fireParam.radius);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0, 0, 0));
}

/// <summary>
/// デストラクタ
/// </summary>
FireMagic::~FireMagic() {
}

/// <summary>
/// 炎魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
/// <param name="color">色</param>
void FireMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	const MagicParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter();
	m_sphereCollider.SetRadius(parameter->fireParam.radius);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0, 0, 0));

	// 方向ベクトルを元に円錐の回転角度を求める
	m_transform.SetRotation(Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::Up, dir));


	m_color = color;
	m_vel = dir * parameter->fireParam.moveSpeed;
	m_lifeTime = parameter->fireParam.lifeTime;

	// 魔法のエフェクトを生成する
	m_pEffect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FireMagic, pos, dir);
	m_pEffect->SetParent(&m_transform);
}


/// <summary>
/// 炎魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FireMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	pos += m_vel*elapsed_time;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 炎魔法を開放する
/// </summary>
void FireMagic::Lost() {

}

/// <summary>
/// 炎魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FireMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	view, proj;
	// 代わりにエフェクトを描画している
	//const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
	//	->GetResource(GeometricPrimitiveID::FireMagic);
	//resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj,DirectX::SimpleMath::Color(1,1,1,0.7f));
	
}

/// <summary>
/// 炎魔法の終了処理を行う
/// </summary>
void FireMagic::Finalize() {
	// エフェクトを終了させる
	m_pEffect->SetUsed(false);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void FireMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void FireMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 落雷魔法と衝突したら消える
	if (other_id == MagicID::ThunderStrike) {
		m_isUsed = false;
	}
}
