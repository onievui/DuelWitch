#include "Field.h"
#include <Framework\DirectX11.h>
#include <Utils\ServiceLocater.h>
#include <Utils\ResourceManager.h>
#include <Utils\MathUtils.h>
#include <Parameters\EffectParameter.h>
#include <Parameters\FieldParameter.h>
#include <Game\Load\PlayParameterLoader.h>
#include <Game\Player\Player.h>
#include <Game\Effect\EffectManager.h>
#include <Game\Effect\EffectID.h>
#include <Game\Effect\FieldShieldEffectEmitter.h>


/// <summary>
/// コンストラクタ
/// </summary>
Field::Field() {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	// モデルのエフェクトを設定する
	const ModelResource* skydome = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetResource(ModelID::Skydome);
	skydome->GetResource()->UpdateEffects([](DirectX::IEffect* effect) {
		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights) {
			// ライトの影響をなくす 
			lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
			lights->SetLightEnabled(0, false);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
		DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basicEffect) {
			// エミッション色を白に設定する 
			basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
		}
	});

	// フィールドの情報を初期化
	m_time = 0.0f;
	m_radius = parameter->defaultScale;
	m_transform.SetPosition(DirectX::SimpleMath::Vector3(0, 0, 0));

	// フィールド情報を設定してサービスロケータに登録する
	m_fieldData.fieldCenter = m_transform.GetPosition();
	m_fieldData.fieldRadius = m_radius;
	m_fieldData.pElements = nullptr;
	ServiceLocater<FieldData>::Register(&m_fieldData);

	// 壁の生成
	m_wall = DirectX::GeometricPrimitive::CreateSphere(context, m_radius*2.0f, 8U, false, true);

	// エフェクトの生成
	IEffectEmitter* effect = ServiceLocater<EffectManager>::Get()->CreateEffect(EffectID::FieldShield,
		m_transform.GetLocalPosition(), DirectX::SimpleMath::Vector3::Zero);
	effect->SetParent(&m_transform);
	m_pEffect = dynamic_cast<FieldShieldEffectEmitter*>(effect);
	if (!m_pEffect) {
		ErrorMessage(L"フィールド衝突エフェクトの生成に失敗しました");
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field() {
	// フィールド情報をサービスロケータから解除する
	ServiceLocater<FieldData>::Unregister();
}

/// <summary>
/// フィールドを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Field::Update(const DX::StepTimer& timer) {
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	m_time += elapsed_time;

	// エフェクト用タイマーを更新する
	for (std::map<const Player*, float>::iterator itr = m_effectTimer.begin(); itr != m_effectTimer.end(); ++itr) {
		itr->second -= elapsed_time;
	}

	// 一定時間経過後、フィールドを徐々に小さくする
	if (m_time > parameter->startScaleDownTime) {
		m_radius = std::max(m_radius - elapsed_time * parameter->scaleDownSpeed, parameter->minScale);
		m_fieldData.fieldRadius = m_radius;
	}
}

/// <summary>
/// フィールドを描画する
/// </summary>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void Field::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) {
	ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
	DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();
	const FieldParameter* parameter = ServiceLocater<PlayParameterLoader>::Get()->GetFieldParameter();

	DirectX::SimpleMath::Vector3 center_pos = m_transform.GetLocalPosition();

	// フィールドの行列を作成する
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(m_radius / parameter->defaultScale);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(center_pos);

	// スカイドームを描画する
	ModelResource* skydome = ServiceLocater<ResourceManager<ModelResource>>::Get()->GetRawResource(ModelID::Skydome);
	skydome->GetResource()->Draw(context, *states, DirectX::SimpleMath::Matrix::CreateScale(1)*world, view, proj);

	// 外壁を描画する
	m_wall->Draw(world, view, proj, DirectX::SimpleMath::Color(30/255.0f, 130/255.0f, 240/255.0f, 1.0f), nullptr, true);

}

/// <summary>
/// プレイヤーとフィールドの当たり判定を行う
/// </summary>
/// <param name="player">プレイヤー</param>
void Field::CollisionCheckPlayer(Player& player) {
	const Collider* collider = player.GetCollider();
	DirectX::SimpleMath::Vector3 center_pos = m_transform.GetLocalPosition();
	if (DirectX::SimpleMath::Vector3::Distance(collider->GetPos(), center_pos) > m_radius) {
		DirectX::SimpleMath::Vector3 direction = collider->GetPos() - center_pos;
		direction.Normalize();
		DirectX::SimpleMath::Vector3 hit_pos = direction * m_radius + center_pos;
		// 衝突点をプレイヤーに渡して処理をさせる
		player.HitField(hit_pos);
		// 衝突エフェクトを発生させる
		CreateEffect(&player, hit_pos);
	}
}

/// <summary>
/// フィールド衝突エフェクトを発生させる
/// </summary>
/// <param name="pPlayer">プレイヤーへのポインタ</param>
/// <param name="pos">エフェクトを発生させる座標</param>
void Field::CreateEffect(const Player* pPlayer, const DirectX::SimpleMath::Vector3& pos) {
	// 同一プレイヤーが発生させた直後なら処理しない
	if (m_effectTimer.count(pPlayer) && m_effectTimer[pPlayer] > 0.0f) {
		return;
	}
	// 始めて発生させるプレイヤーなら登録する
	if (!m_effectTimer.count(pPlayer)) {
		m_effectTimer.emplace(pPlayer, 0.0f);
	}
	// エフェクトを発生させてタイマーをセットする
	m_pEffect->AddEffect(pos);
	const EffectParameter::field_shield_param& parameter = ServiceLocater<PlayParameterLoader>::Get()->GetEffectParameter()->fieldShieldParam;
	float interval = parameter.lifeTime / parameter.particleNum *2.0f;
	m_effectTimer[pPlayer] = interval;
}
