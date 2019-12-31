#pragma once
#ifndef FADE_DEFINED
#define FADE_DEFINED


#include <Framework\StepTimer.h>


// フェードクラス
class Fade {
public:
	// フェードの種類
	enum class State {
		NoFade,
		FadeIn,
		FadeOut,
		// アルファ値取得用
		FadeInAlpha = FadeOut,
		FadeOutAlpha = FadeIn,
	};

public:
	// コンストラクタ
	Fade();

public:
	// フェードクラスを初期化する
	void Initialize(State state, float endTime, float endAlpha,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::Black));
	// フェード状態を更新する
	void Update(const DX::StepTimer& timer);
	// フェードを描画する
	void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// 時間をリセットする
	void ResetTime();
	// 時間を飛ばす
	void SkipTime(float timeRate = 1.0f);
	// 再開する
	void Start();
	// 一時停止する
	void Stop();
	// 実行中かどうか取得する
	bool IsRunning() const { return m_isRunning; }
	// 完了しているかどうか取得する
	bool IsFinished() const;
	// 現在のアルファ値を取得する
	float GetAlpha() const;

private:
	// フェードの種類
	State                      m_state;
	// 実行中かどうかのフラグ
	bool                       m_isRunning;
	// 現在の時間
	float                      m_time;
	// 終了時間
	float                      m_endTime;
	// 最終的なアルファ値
	float                      m_endAlpha;
	// フェードカラー
	DirectX::SimpleMath::Color m_fadeColor;
};


#endif // !FADE_DEFINED



