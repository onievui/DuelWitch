#include "RenderCommand.h"
#include <Framework\DirectX11.h>
#include <Utils\ResourceManager.h>
#include <Utils\ServiceLocater.h>
#include <Utils\MathUtils.h>


/// <summary>
/// 赤HPの更新
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="timer">ステップタイマー</param>
void RenderCommand::UpdateRedHpBar(Player& player, const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	const PlayerStatus& status = GetStatus(player);

	// ダメージを受けた場合はタイマーを起動する
	if (m_preHp > status.hp) {
		m_preHp = status.hp;
		m_redHpTime = RED_HP_TIME;
	}

	// ダメージ直後でない場合
	if (m_redHpTime <= 0.0f) {
		// 赤HP部分が残っていたら減少させる
		if (m_redHp > status.hp) {
			m_redHp -= RED_HP_SPEED * elapsed_time;
			if (m_redHp < status.hp) {
				m_redHp = status.hp;
			}
		}
	}
	// ダメージを受けた後の場合
	else {
		// 一定時間経過するまで待つ
		m_redHpTime -= elapsed_time;
	}
}

/// <summary>
/// プレイヤーモデルの描画
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void RenderCommand::RenderPlayerModel(const Player& player, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	const PlayerStatus& ref_status = GetStatus(player);
	
	// ダメージ後はプレイヤーを点滅させる
	const bool not_damaged = ref_status.damageTimer <= 0.0f;
	const bool time_sin_positive = sin(ref_status.damageTimer*Math::PI2 * 2) > 0;
	if (not_damaged || time_sin_positive) {
		const std::unique_ptr<DirectX::Model>& model = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Bloom)->GetResource();
		const std::unique_ptr<DirectX::Model>& model2 = ServiceLocater<ResourceManager<ModelResource>>::Get()->
			GetResource(ModelID::Chara)->GetResource(ref_status.charaId);

		ID3D11DeviceContext* context = ServiceLocater<DirectX11>::Get()->GetContext().Get();
		const DirectX::CommonStates* states = ServiceLocater<DirectX::CommonStates>::Get();

		// キャラモデルのオフセット
		DirectX::SimpleMath::Matrix mat = DirectX::SimpleMath::Matrix::CreateRotationX(-Math::HarfPI);
		mat *= DirectX::SimpleMath::Matrix::CreateScale(0.5f);
		mat *= DirectX::SimpleMath::Matrix::CreateTranslation(0, 0.625f, -0.2f);
		mat *= GetTransform(player).GetMatrix();

		// モデルを描画する
		model->Draw(context, *states, GetTransform(player).GetMatrix(), view, proj);
		model2->Draw(context, *states, mat, view, proj);

		// 当たり判定を描画する
		//GetCollider(player).Render(view, proj, DirectX::SimpleMath::Color(1, 1, 1, 0.3f), true);
	}
}
