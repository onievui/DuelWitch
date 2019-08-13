#include "Player.h"
#include <Framework\DirectX11.h>
#include <Utils\MathUtils.h>
#include "Command.h"
#include "MoveCommand.h"
#include "AIMoveCommand.h"
#include "CastMagicCommand.h"
#include "AICastMagicCommand.h"
#include "IMagic.h"
#include "MagicManager.h"
#include "MagicFactory.h"
#include "Camera.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager">魔法マネージャ</param>
/// <param name="id">プレイヤーID</param>
/// <param name="pos">初期座標</param>
/// <param name="direction">進行方向</param>
Player::Player(MagicManager* magicManager, PlayerID id, const DirectX::SimpleMath::Vector3& pos, MoveDirection direction)
	: m_model()
	, m_states()
	, m_id(id)
	, m_direction(direction)
	, m_haveElements()
	, m_transform(pos, DirectX::SimpleMath::Vector3(0, (m_direction == MoveDirection::Forward ? 0 : Math::PI), 0))
	, m_sphereCollider(&m_transform, 1.5f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager(magicManager)
	, m_pCamera() {
	if (id == PlayerID::Player1) {
		m_moveCommand = std::make_unique<MoveCommand>();
		m_castCommand = std::make_unique<CastMagicCommand>();
	}
	else {
		m_moveCommand = std::make_unique<AIMoveCommand>();
		m_castCommand = std::make_unique<AICastMagicCommand>();
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
}

/// <summary>
/// プレイヤーを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Player::Update(const DX::StepTimer& timer) {
	// 移動を行う
	m_moveCommand->Execute(*this, timer);

	// 魔法を発動する
	m_castCommand->Execute(*this, timer);

	m_damageTimer -= static_cast<float>(timer.GetElapsedSeconds());
}

/// <summary>
/// プレイヤーを解放する
/// </summary>
void Player::Lost() {
	m_states.reset();
	m_model.reset();
}

/// <summary>
/// プレイヤーを生成する
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <param name="directory">ディレクトリ名</param>
void Player::Create(const std::wstring& fileName, const std::wstring& directory) {
	// デバイスの取得
	ID3D11Device* device = DirectX11::Get().GetDevice().Get();

	// コモンステートを作成する
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// エフェクトファクトリーを作成する
	std::unique_ptr<DirectX::EffectFactory> fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// 読み込むのファイルのディレクトリを設定する
	dynamic_cast<DirectX::EffectFactory*>(fxFactory.get())->SetDirectory(directory.c_str());
	// CMOを読み込んでモデルを作成する
	m_model = DirectX::Model::CreateFromCMO(device, (directory+L"/"+fileName).c_str(), *fxFactory);
	// エフェクトを設定する
	m_model->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			lights->SetLightingEnabled(true);
			lights->SetPerPixelLighting(true);
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, DirectX::Colors::AntiqueWhite);
			lights->SetAmbientLightColor(DirectX::Colors::AntiqueWhite*0.3f);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	});

	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Protected/element1.png", nullptr, m_textures[0].GetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Protected/element2.png", nullptr, m_textures[1].GetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Protected/element3.png", nullptr, m_textures[2].GetAddressOf());
}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	ID3D11DeviceContext* context = DirectX11::Get().GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2*2)>0) {
		m_model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
}

void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, DirectX::SpriteBatch* spriteBatch) const {
	ID3D11DeviceContext* context = DirectX11::Get().GetContext().Get();
	if (m_damageTimer <= 0.0f || sin(m_damageTimer*Math::PI2 * 2) > 0) {
		m_model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
		m_sphereCollider.Render(view, proj);
	}
	if (m_id == PlayerID::Player2)
		return;
	int i = m_haveElements.size() - 1;
	for (std::list<ElementID>::const_reverse_iterator itr = m_haveElements.rbegin(); itr != m_haveElements.rend(); ++itr) {
		spriteBatch->Draw(m_textures[static_cast<int>(*itr)].Get(), DirectX::SimpleMath::Vector2(20 + i * 40.0f, 630.0f), nullptr,
			DirectX::Colors::White, 0, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.5f, 1.5f));
		--i;
	}

}

/// <summary>
/// プレイヤーの行列を取得する
/// </summary>
/// <returns>
/// プレイヤーの行列
/// </returns>
const DirectX::SimpleMath::Matrix& Player::GetMatrix() const {
	return m_world;
}

/// <summary>
/// プレイヤーの当たり判定を取得する
/// </summary>
/// <returns>
/// 当たり判定
/// </returns>
const SphereCollider* Player::GetCollider() const {
	return &m_sphereCollider;
}

/// <summary>
/// プレイヤーIDを取得する
/// </summary>
/// <returns>
/// プレイヤーID
/// </returns>
PlayerID Player::GetPlayerID() const {
	return m_id;
}

/// <summary>
/// 敵プレイヤーを設定する
/// </summary>
/// <param name="otherPlayer">敵プレイヤーへのポインタ</param>
void Player::SetOtherPlayer(Player* otherPlayer) {
	m_otherPlayer = otherPlayer;
}

/// <summary>
/// カメラを設定する
/// </summary>
/// <param name="camera">カメラへのポインタ</param>
void Player::SetCamera(Camera* camera) {
	m_pCamera = camera;
}

/// <summary>
/// エレメントの取得処理を行う
/// </summary>
/// <param name="elementId">エレメントID</param>
void Player::GetElement(ElementID elementId) {
	m_haveElements.push_back(elementId);
}

/// <summary>
/// プレイヤー同士の衝突処理を行う
/// </summary>
/// <param name="player">相手プレイヤー</param>
void Player::HitPlayer(const Player& player) {
	constexpr float reflect_distance = 0.25f;

	DirectX::SimpleMath::Vector3 my_pos = m_transform.GetPosition();
	const DirectX::SimpleMath::Vector3& other_pos = player.m_transform.GetPosition();
	// 相手プレイヤーと判定方向に進む
	float angle = std::atan2f(my_pos.y - other_pos.y, my_pos.x - other_pos.x);
	my_pos += DirectX::SimpleMath::Vector3(std::cosf(angle), std::sinf(angle), 0.0f)*reflect_distance;
	m_transform.SetPosition(my_pos);
}

/// <summary>
/// 魔法との衝突処理を行う
/// </summary>
/// <param name="magic">魔法</param>
void Player::HitMagic(const IMagic* magic) {
	if (magic->GetID() == MagicID::Thunder) {
		return;
	}
	if (m_damageTimer <= 0.0f) {
		m_damageTimer = 3.0f;
	}
}

