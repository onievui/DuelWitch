#include "ThunderStrikeMagic.h"
#include <Framework/DirectX11.h>
#include "MagicFactory.h"
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
ThunderStrikeMagic::ThunderStrikeMagic()
	: Magic(MagicID::ThunderStrike)
	, m_object() {
	m_sphereCollider.SetRadius(THUNDER_STRIKE_MAGIC_RADIUS);
	m_sphereCollider.SetOffset(DirectX::SimpleMath::Vector3(0,-THUNDER_STRIKE_MAGIC_LENGTH/4,0));

}

/// <summary>
/// デストラクタ
/// </summary>
ThunderStrikeMagic::~ThunderStrikeMagic() {
}

/// <summary>
/// 落雷魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void ThunderStrikeMagic::Update(const DX::StepTimer& timer) {
	m_lifeTime -= float(timer.GetElapsedSeconds());
	if (m_lifeTime < 0) {
		m_isUsed = false;
	}
	DirectX::SimpleMath::Vector3 pos = m_transform.GetPosition();
	pos += m_vel;
	m_transform.SetPosition(pos);

	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 落雷魔法を開放する
/// </summary>
void ThunderStrikeMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// 落雷魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="color">色</param>
void ThunderStrikeMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateCylinder(DirectX11::Get().GetContext().Get(), THUNDER_STRIKE_MAGIC_LENGTH,
		THUNDER_STRIKE_MAGIC_RADIUS*2);
	m_lifeTime = 5.0f;
}

/// <summary>
/// 落雷魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void ThunderStrikeMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, DirectX::Colors::White, nullptr, true);
	m_sphereCollider.Render(view, proj, m_color);
}

/// <summary>
/// プレイヤーとの衝突処理
/// </summary>
/// <param name="collider">プレイヤーの当たり判定</param>
void ThunderStrikeMagic::HitPlayer(const SphereCollider& collider) {
	collider;
	m_isUsed = false;
}

/// <summary>
/// 魔法との衝突処理
/// </summary>
/// <param name="other">衝突した魔法</param>
void ThunderStrikeMagic::HitMagic(const IMagic* other) {
	auto other_id = other->GetID();
	// 氷魔法と衝突したら跳ね返る
	if (other_id == MagicID::Freeze) {
		auto collider = other->GetCollider();
		auto& rot = collider->GetTransform()->GetRotation();
		auto offset = DirectX::SimpleMath::Vector3::Transform(collider->GetOffset(),
			DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z));
		auto pos = offset + collider->GetTransform()->GetPosition();
		auto direction = m_transform.GetPosition() - pos;
		direction.y *= 0.0f;
		direction.Normalize();

		float rot_x = 0;
		if (direction.z > 0) {
			rot_x = acos(direction.y);
		}
		else {
			rot_x = -acos(direction.y);
		}
		float rot_z = 0;
		if (direction.x > 0) {
			rot_z = -acos(1 - direction.x);
		}
		else {
			rot_z = acos(1 + direction.x);
		}
		m_transform.SetRotation(DirectX::SimpleMath::Vector3(rot_x, 0, rot_z));
		m_vel = direction * m_vel.Length();
		//跳ね返り後は相手の魔法扱いになる
		m_playerId = other->GetPlayerID();
	}
}
