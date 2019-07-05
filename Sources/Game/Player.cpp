#include "Player.h"
#include <Framework\DirectX11.h>
#include "MagicManager.h"
#include "MagicFactory.h"
#include "Camera.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="magicManager">魔法マネージャ</param>
Player::Player(MagicManager* magicManager)
	: m_model()
	, m_states()
	, m_transform()
	, m_sphereCollider(&m_transform, 1.5f, DirectX::SimpleMath::Vector3(0,0.5f,0)) 
	, m_pMagicManager(magicManager)
	, m_pCamera() {
	m_mouseTracker = std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
}

/// <summary>
/// モデルオブジェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Player::Update(const DX::StepTimer& timer) {
	// 移動を行う
	Move(timer);
	// 魔法を発動する
	CastMagic(timer);

}

/// <summary>
/// モデルオブジェクトを解放する
/// </summary>
void Player::Lost() {
	m_states.reset();
	m_model.reset();
}

/// <summary>
/// モデルオブジェクトを生成する
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <param name="directory">ディレクトリ名</param>
void Player::Create(const std::wstring& fileName, const std::wstring& directory) {
	// デバイスの取得
	auto device = DirectX11::Get().GetDevice().Get();

	// コモンステートを作成する
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// エフェクトファクトリーを作成する
	auto fxFactory = std::make_unique<DirectX::EffectFactory>(device);
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

}

/// <summary>
/// モデルオブジェクトを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">プロジェクション行列</param>
void Player::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	auto context = DirectX11::Get().GetContext().Get();
	m_model->Draw(context, *m_states, m_transform.GetMatrix(), view, proj);
	m_sphereCollider.Render(view, proj);
}

/// <summary>
/// モデルオブジェクトの行列を取得する
/// </summary>
/// <returns>
/// モデルオブジェクトの行列
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
/// カメラを設定する
/// </summary>
/// <param name="camera">カメラへのポインタ</param>
void Player::SetCamera(Camera* camera) {
	m_pCamera = camera;
}

/// <summary>
/// 移動を行う
/// </summary>
/// <param name="timer">タイマー</param>
void Player::Move(const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());

	auto keyState = DirectX::Keyboard::Get().GetState();

	constexpr float moveSpeed = 8.0f;
	constexpr float rotSpeed = 2.0f;
	constexpr float rotZLimit = Math::QuarterPI*0.5f;
	constexpr float rotXLimit = Math::QuarterPI*0.5f;
	constexpr float rotYLimit = Math::QuarterPI*0.25f;
	constexpr float lerpSpeed = 0.025f;

	auto pos = m_transform.GetPosition();
	auto rot = m_transform.GetRotation();

	// 移動
	if (keyState.A || keyState.Left) {
		//pos += DirectX::SimpleMath::Vector3(sinf(rot.y + Math::HarfPI)*moveSpeed*elapsedTime,
		//	0.0f, cosf(rot.y + Math::HarfPI)*moveSpeed*elapsedTime);
		rot.z = Math::Lerp(rot.z, -rotZLimit, lerpSpeed);
		rot.y = Math::Lerp(rot.y, rotYLimit, lerpSpeed);
	}
	else if (keyState.D || keyState.Right) {
		//pos += DirectX::SimpleMath::Vector3(sinf(rot.y - Math::HarfPI)*moveSpeed*elapsedTime,
		//	0.0f, cosf(rot.y - Math::HarfPI)*moveSpeed*elapsedTime);
		rot.z = Math::Lerp(rot.z, rotZLimit, lerpSpeed);
		rot.y = Math::Lerp(rot.y, -rotYLimit, lerpSpeed);
	}
	//押していないときは戻す
	else {
		rot.z = Math::Lerp(rot.z, 0.0f, lerpSpeed);
		rot.y = Math::Lerp(rot.y, 0.0f, lerpSpeed);
	}

	if (keyState.W || keyState.Up) {
		//pos += DirectX::SimpleMath::Vector3(0.0f, moveSpeed*elapsedTime, 0.0f);
		rot.x = Math::Lerp(rot.x, -rotXLimit, lerpSpeed);
	}
	else if (keyState.S || keyState.Down) {
		//pos += DirectX::SimpleMath::Vector3(0.0f, -moveSpeed*elapsedTime, 0.0f);
		rot.x = Math::Lerp(rot.x, rotXLimit, lerpSpeed);
	}
	//押していないときは戻す
	else {
		rot.x = Math::Lerp(rot.x, 0.0f, lerpSpeed);
	}

	DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	pos += DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ*moveSpeed*elapsedTime,
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion));

	m_transform.SetPosition(pos);
	m_transform.SetRotation(rot);
	m_world = m_transform.GetMatrix();
}

/// <summary>
/// 魔法を唱える
/// </summary>
/// <param name="timer">タイマー</param>
void Player::CastMagic(const DX::StepTimer& timer) {
	float elapsedTime = float(timer.GetElapsedSeconds());

	//static DirectX::Mouse::ButtonStateTracker tracker;
	auto mouse_state = DirectX::Mouse::Get().GetState();
	m_mouseTracker->Update(mouse_state);
	// タッチパッドだと左クリックが正常に反応していない？
	if (m_mouseTracker->leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED) {
		// レイの作成
		auto ray = m_pCamera->ScreenPointToRay(DirectX::SimpleMath::Vector3((float)mouse_state.x, (float)mouse_state.y, 0));
		// 平面の作成
		auto plane = CreatePlaneForMagic();
		float distance;
		if (ray.Intersects(plane, distance)) {
			auto ray_pos = ray.position + ray.direction * distance;
			auto& player_pos = m_transform.GetPosition();
			auto direction = ray_pos - player_pos;
			direction.Normalize();
			m_pMagicManager->CreateMagic(MagicFactory::MagicID::Fire, player_pos, direction);
		}
	}
}

/// <summary>
/// 魔法のためのレイ用平面の作成
/// </summary>
/// <returns>
/// 平面
/// </returns>
DirectX::SimpleMath::Plane Player::CreatePlaneForMagic() {
	auto& rot = m_transform.GetRotation();
	auto quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	auto plane_normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ,
		DirectX::SimpleMath::Matrix::CreateFromQuaternion(quaternion));
	auto plane_pos = m_transform.GetPosition() + DirectX::SimpleMath::Vector3::UnitZ * 20;
	auto plane = DirectX::SimpleMath::Plane(plane_pos, DirectX::SimpleMath::Vector3::UnitZ);
	return plane;
}
