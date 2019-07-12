#include "FireMagic.h"
#include <Framework/DirectX11.h>
#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
FireMagic::FireMagic()
	: m_object()
	, m_playerId()
	, m_transform()
	, m_vel()
	, m_sphereCollider(&m_transform, FIRE_MAGIC_RADIUS)
	, m_color()
	, m_isUsed(false) {
}

/// <summary>
/// デストラクタ
/// </summary>
FireMagic::~FireMagic() {
}

/// <summary>
/// 炎魔法を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void FireMagic::Update(const DX::StepTimer& timer) {
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
/// 炎魔法を開放する
/// </summary>
void FireMagic::Lost() {
	m_object.reset();
}

/// <summary>
/// 炎魔法を生成する
/// </summary>
/// <param name="playerId">プレイヤーID</param>
/// <param name="pos">座標</param>
/// <param name="vel">速度</param>
/// <param name="color">色</param>
void FireMagic::Create(PlayerID playerId, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& vel,
	const DirectX::SimpleMath::Vector4& color) {
	m_playerId = playerId;
	m_transform.SetPosition(pos);
	m_color = color;
	m_vel = vel;
	m_object = DirectX::GeometricPrimitive::CreateSphere(DirectX11::Get().GetContext().Get(), FIRE_MAGIC_RADIUS);
	m_lifeTime = 10.0f;
}

/// <summary>
/// 炎魔法を描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void FireMagic::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	m_object->Draw(m_world, view, proj, m_color, nullptr, true);
}

/// <summary>
/// 炎魔法の行列を取得する
/// </summary>
/// <returns>
/// 行列
/// </returns>
const DirectX::SimpleMath::Matrix& FireMagic::GetMatrix() const {
	return m_world;
}

/// <summary>
/// 炎魔法の当たり判定を取得する
/// </summary>
/// <returns>
/// 当たり判定
/// </returns>
const SphereCollider* FireMagic::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// プレイヤーIDを取得する
/// </summary>
/// <returns>
/// プレイヤーID
/// </returns>
PlayerID FireMagic::GetPlayerID() const {
	return m_playerId;
}

/// <summary>
/// 炎魔法を使用しているかどうか取得する
/// </summary>
/// <returns>
/// true : 使用している
/// false : 使用していない
/// </returns>
bool FireMagic::IsUsed() const {
	return m_isUsed;
}

/// <summary>
/// 炎魔法を使用するかどうか設定する
/// </summary>
/// <param name="isUsed">true : 使用する, false : 使用しない</param>
void FireMagic::IsUsed(bool isUsed) {
	m_isUsed = isUsed;
}
