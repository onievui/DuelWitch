#include "Fade.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>


/// <summary>
/// コンストラクタ
/// </summary>
Fade::Fade() 
	: m_state(State::NoFade)
	, m_isRunning(false)
	, m_time()
	, m_endTime()
	, m_endAlpha()
	, m_fadeColor(DirectX::Colors::Black) {
}

/// <summary>
/// フェードクラスの初期化
/// </summary>
/// <param name="state">フェードの種類</param>
/// <param name="endTime">終了時間</param>
/// <param name="endAlpha">最終的なアルファ値</param>
/// <param name="color">フェードカラー</param>
void Fade::Initialize(State state, float endTime, float endAlpha, const DirectX::SimpleMath::Color& color) {
	m_state = state;
	m_isRunning = true;
	m_time = 0;
	m_endTime = std::max(endTime, 0.0f);
	m_endAlpha = Math::Clamp(0.0f, endAlpha, 1.0f);
	m_fadeColor = color;
}

/// <summary>
/// フェード状態を更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void Fade::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// 停止中なら処理しない
	if (!m_isRunning) {
		return;
	}

	// 経過時間をカウントする
	if (m_time < m_endTime) {
		m_time += elapsed_time;
		m_time = Math::Clamp(0.0f, m_time, m_endTime);
	}
}

/// <summary>
/// フェードを描画する
/// </summary>
/// <param name="spriteBatch">スプライトバッチ</param>
void Fade::Render(DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Fade);

	// 描画する色とアルファ値を計算する
	DirectX::SimpleMath::Color color = m_fadeColor;
	color.w = GetAlpha();

	// アルファ値が0より高いなら描画する
	if (color.w > 0.0f) {
		spriteBatch->Draw(texture->GetResource().Get(),
			DirectX::SimpleMath::Vector2::Zero,
			nullptr, color, 0,
			DirectX::SimpleMath::Vector2::Zero,
			DirectX::SimpleMath::Vector2(1.0f, 1.0f));
	}
}

/// <summary>
/// 時間をリセットする
/// </summary>
void Fade::ResetTime() {
	m_time = 0.0f;
}

/// <summary>
/// 時間を飛ばす
/// </summary>
/// <param name="timeRate">飛ばす時間の割合</param>
void Fade::SkipTime(float timeRate) {
	m_time += m_endTime * timeRate;
	m_time = Math::Clamp(m_time, 0.0f, m_endTime);
}

/// <summary>
/// 再開する
/// </summary>
void Fade::Start() {
	m_isRunning = true;
}

/// <summary>
/// 一時停止する
/// </summary>
void Fade::Stop() {
	m_isRunning = false;
}

/// <summary>
/// 完了しているかどうか取得する
/// </summary>
/// <returns>
/// true : 完了している
/// false : 完了してない
/// </returns>
bool Fade::IsFinished() const {
	return m_time >= m_endTime;
}

/// <summary>
/// 現在のアルファ値を取得する
/// </summary>
/// <returns>
/// アルファ値
/// </returns>
float Fade::GetAlpha() const {
	// フェードしない場合はそのままアルファ値を返す
	if (m_state == State::NoFade) {
		return m_endAlpha;
	}

	// 変化状況を計算する
	if (m_endTime <= 0.0f) {
		// 設定された時間が0以下の場合は変化済みと判断する
		return m_endAlpha;
	}
	else {
		float t = m_time / m_endTime;
		// フェードの種類によって向きを変える
		if (m_state == State::FadeIn) {
			return Math::Lerp(1.0f, m_endAlpha, t);
		}
		else {
			return Math::Lerp(0.0f, m_endAlpha, t);
		}
	}
}
