#include "FreezeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include "MagicID.h"
#include <Game\Collision\SphereCollider.h>


/// <summary>
/// コンストラクタ
/// </summary>
FreezeMagic::FreezeMagic()
	: Magic()
	, m_pPlayerPos()
	, m_time()
	, m_rotateRadius() {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;
	m_collider = std::make_unique<SphereCollider>(&m_transform, parameter.radius);
}

/// <summary>
/// デストラクタ
/// </summary>
FreezeMagic::~FreezeMagic() {
}

/// <summary>
/// 氷魔法を生成する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
void FreezeMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	static_cast<SphereCollider*>(m_collider.get())->SetRadius(parameter.radius);
	m_pPlayerPos = &pos;
	m_time = 0.0f;
	m_rotateRadius = 0.0f;
	m_color = DirectX::Colors::SkyBlue + DirectX::SimpleMath::Color(0, 0, 0, 0.8f);
	// 向きをXZ方向に変換する
	m_vel = dir;
	m_vel.y = 0.0f;

	m_lifeTime = parameter.lifeTime;
}

/// <summary>
/// 氷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FreezeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	// 生存時間が無くなったら消滅する
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}

	// タイマーを進める
	m_time += elapsed_time;

	// プレイヤーの位置を中心とする
	DirectX::SimpleMath::Vector3 center_pos = *m_pPlayerPos;
	// 回転したときの位置を求める
	m_rotateRadius = Math::Lerp(m_rotateRadius, parameter.rotateRadius, 0.2f);
	DirectX::SimpleMath::Vector3 add_pos = DirectX::SimpleMath::Vector3::Transform(
		m_vel*m_rotateRadius,
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, m_time*parameter.rotateSpeed));
	
	// プレイヤーの位置に回転した位置を足す
	m_transform.SetPosition(center_pos + add_pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 氷魔法を開放する
/// </summary>
void FreezeMagic::Lost() {

}

/// <summary>
/// 氷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FreezeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::FreezeMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// ダメージを取得する
/// </summary>
/// <returns>
/// ダメージ量
/// </returns>
float FreezeMagic::GetPower() const {
	const MagicParameter::freeze_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->freezeParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void FreezeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void FreezeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 炎魔法と衝突したら消える
	if (other_id == MagicID::Fire) {
		m_isUsed = false;
	}
}
