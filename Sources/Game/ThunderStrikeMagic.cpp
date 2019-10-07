#include "ThunderStrikeMagic.h"
#include <Framework/DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\MagicParameter.h>
#include "PlayParameterLoader.h"
#include "MagicID.h"
#include "CapsuleCollider.h"


/// <summary>
/// コンストラクタ
/// </summary>
ThunderStrikeMagic::ThunderStrikeMagic()
	: Magic() {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;
	m_collider = std::make_unique<CapsuleCollider>(&m_transform, parameter.radius,
		DirectX::SimpleMath::Vector3(0, parameter.height*0.5f, 0), DirectX::SimpleMath::Vector3(0, -parameter.height*0.5f, 0));

}

/// <summary>
/// デストラクタ
/// </summary>
ThunderStrikeMagic::~ThunderStrikeMagic() {
}

/// <summary>
/// 落雷魔法を生成する
/// </summary>
/// <param name="magicInfo">魔法に関する情報</param>
/// <param name="pos">座標</param>
/// <param name="dir">方向</param>
void ThunderStrikeMagic::Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;

	m_info = magicInfo;
	m_transform.SetPosition(pos);
	CapsuleCollider* collider = static_cast<CapsuleCollider*>(m_collider.get());
	collider->SetRadius(parameter.radius);
	collider->SetStartPos(DirectX::SimpleMath::Vector3(0, parameter.height*0.5f, 0));
	collider->SetEndPos(DirectX::SimpleMath::Vector3(0, -parameter.height*0.5f, 0));
	m_color = DirectX::Colors::Yellow;
	m_vel = dir * parameter.moveSpeed;
	m_lifeTime = parameter.lifeTime;
}

/// <summary>
/// 落雷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ThunderStrikeMagic::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());
	m_lifeTime -= elapsed_time;
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetLocalPosition();
	pos += m_vel * elapsed_time;;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 落雷魔法を開放する
/// </summary>
void ThunderStrikeMagic::Lost() {

}

/// <summary>
/// 落雷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void ThunderStrikeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const GeometricPrimitiveResource* resource = ServiceLocater<ResourceManager<GeometricPrimitiveResource>>::Get()
		->GetResource(GeometricPrimitiveID::ThunderStrikeMagic);
	resource->GetResource()->Draw(m_world, view, proj, m_color, nullptr, true);
	//m_sphereCollider.Render(view, proj);
}

/// <summary>
/// ダメージを取得する
/// </summary>
/// <returns>
/// ダメージ量
/// </returns>
float ThunderStrikeMagic::GetPower() const {
	const MagicParameter::thunder_strike_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetMagicParameter()->thunderStrikeParam;

	return parameter.power*m_info.powerRate;
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void ThunderStrikeMagic::HitPlayer(const Collider* collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void ThunderStrikeMagic::HitMagic(const IMagic* other) {
	MagicID other_id = other->GetID();
	// 氷魔法と衝突したら跳ね返る
	if (other_id == MagicID::Freeze) {
		DirectX::SimpleMath::Vector3 direction = m_transform.GetLocalPosition() - other->GetCollider()->GetPos();
		direction.y *= 0.0f;
		direction.Normalize();

		m_transform.SetRotation(Math::CreateQuaternionFromVector3(DirectX::SimpleMath::Vector3::Up, direction));

		m_vel = direction * m_vel.Length();
		//跳ね返り後は相手の魔法扱いになる
		m_info.playerId = other->GetPlayerID();
	}
}
