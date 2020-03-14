#include "PlayerManager.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>
#include <Utils\ResourceManager.h>
#include <Utils\Resource.h>
#include "Player.h"
#include "PlayerID.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager() {
	// プレイヤーデータをサービスロケータに登録する
	ServiceLocater<PlayerData>::Register(&m_playerData);
	// プレイヤーデータの容量を設定する
	m_playerData.transforms.resize(static_cast<int>(PlayerID::Num));

	// プレイヤーの初期座標
	DirectX::SimpleMath::Vector3 player_pos(0, 0, -75);
	// プレイヤーの初期座標をずらすための回転を生成する
	DirectX::SimpleMath::Quaternion player_pos_rot =
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::UnitY, Math::PI2 / static_cast<int>(PlayerID::Num));

	// プレイヤーを生成する
	for (int i = 0; i < static_cast<int>(PlayerID::Num); ++i) {
		// 少しづつ回転させる
		player_pos = DirectX::SimpleMath::Vector3::Transform(player_pos, player_pos_rot);
		m_players.emplace_back(std::make_unique<Player>(static_cast<PlayerID>(i), player_pos));
	}
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager() {
	m_players.clear();
	ServiceLocater<PlayerData>::Unregister();
}

/// <summary>
/// プレイヤーマネージャを初期化する
/// </summary>
/// <param name="pMagicManager">魔法マネージャへのポインタ</param>
/// <param name="camera">カメラへのポインタ</param>
void PlayerManager::Initialize(MagicManager* pMagicManager, Camera* camera) {
	// プレイヤーを初期化する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->Initialize(pMagicManager, camera, m_players);
	}

	// 定数バッファの作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Player1EffectBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ServiceLocater<DirectX11>::Get()->GetDevice().Get()->CreateBuffer(&bd, nullptr, m_cBuffer.GetAddressOf());

	// シェーダ用定数の初期化
	m_player1EffectBuffer.time = DirectX::SimpleMath::Vector4(0, 0, 0, 0);
}

/// <summary>
/// プレイヤーマネージャを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void PlayerManager::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// 生存していないプレイヤーを削除する
	m_players.erase(std::remove_if(m_players.begin(), m_players.end(), std::mem_fn(&Player::IsDead)), m_players.end());

	// プレイヤーを更新する
	for (std::vector<std::unique_ptr<Player>>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr) {
		(*itr)->SetOtherPlayers(m_players);
		(*itr)->Update(timer);
	}

	// シェーダ用定数を更新する
	m_player1EffectBuffer.time.x += elapsed_time;
}

/// <summary>
/// プレイヤーを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <param name="spriteBatch">スプライトバッチ</param>
void PlayerManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
	DirectX::SpriteBatch* spriteBatch) const {
	// プレイヤーを描画する
	for (std::vector<std::unique_ptr<Player>>::const_iterator itr = m_players.cbegin(); itr != m_players.cend(); ++itr) {
		(*itr)->Render(view, proj, spriteBatch);
	}
}

/// <summary>
/// プレイヤー1の画面エフェクトを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void PlayerManager::RenderPlayer1Effect(DirectX::SpriteBatch* spriteBatch) {
	// プレイヤー1を取得する
	std::vector<std::unique_ptr<Player>>::const_iterator player1_itr = std::find_if(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return (player->GetPlayerID() == PlayerID::Player1); });

	// 取得できない場合は処理を抜ける
	if (player1_itr == m_players.cend()) {
		return;
	}

	// ブースト状態ならエフェクトを描画する
	if (!(*player1_itr)->IsBoosting()) {
		return;
	}

	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();

	//定数バッファの内容更新
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_player1EffectBuffer, 0, 0);

	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, ServiceLocater<DirectX::CommonStates>::Get()->NonPremultiplied(),
		nullptr, nullptr, nullptr, [&]() {
		const PixelShaderResource* shader = ServiceLocater<ResourceManager<PixelShaderResource>>::Get()->GetResource(PixelShaderID::Boost);
		context->PSSetShader(shader->GetResource().Get(), nullptr, 0);
		context->PSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());
	});


	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::BoostEffect);
	spriteBatch->Draw(texture->GetResource().Get(), DirectX::SimpleMath::Vector2(640, 360), nullptr, DirectX::Colors::SkyBlue,
		0.0f, texture->GetCenter(), DirectX::SimpleMath::Vector2(1280 / 256.0f, 720 / 256.0f));
	spriteBatch->End();
}


/// <summary>
/// プレイヤーを取得する
/// </summary>
/// <returns>
/// プレイヤー
/// </returns>
std::vector<std::unique_ptr<Player>>* PlayerManager::GetPlayers() {
	return &m_players;
}

/// <summary>
/// プレイヤー1が勝利しているか判定する
/// </summary>
/// <returns>
/// true : 勝利している
/// false : 勝利していない
/// </returns>
bool PlayerManager::Player1Win() {
	// 生存しているプレイヤーがいないなら（同時に倒れた場合）勝利扱いにする
	if (m_players.size() == 0) {
		return true;
	}
	// プレイヤー1だけが生存しているなら勝利
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() == PlayerID::Player1; });
}

/// <summary>
/// プレイヤー1が敗北しているか判定する
/// </summary>
/// <returns>
/// true : 敗北している
/// false : 敗北していない
/// </returns>
bool PlayerManager::Player1Lose() {
	// 生存しているプレイヤーがいないなら（同時に倒れた場合）勝利扱いにする
	if (m_players.size() == 0) {
		return false;
	}

	// プレイヤー1が生存していないなら敗北
	return std::all_of(m_players.cbegin(), m_players.cend(),
		[](const std::unique_ptr<Player>& player) { return player->GetPlayerID() != PlayerID::Player1; });
}
