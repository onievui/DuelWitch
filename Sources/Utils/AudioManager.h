#pragma once
#ifndef AUDIO_MANAGER_DEFINED
#define AUDIO_MANAGER_DEFINED


#include <Framework\StepTimer.h>
#include "ResourceID.h"


/// <summary>
/// オーディオマネージャクラス
/// </summary>
class AudioManager {
public:
	// コンストラクタ
	AudioManager();
	// デストラクタ
	~AudioManager();

public:
	// オーディオエンジンを取得する
	DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	// オーディオを更新する
	void Update(const DX::StepTimer& timer);
	// サウンドを再生する
	void PlaySound(SoundID id, int index = 0, float volume = 1.0f);
	// BGMを再生する
	void PlayBgm(BgmID id, int index = 0, float volume = 1.0f);
	// サウンドの再生を止める
	void StopSound(SoundID id, int index = 0);
	// BGMの再生を止める
	void StopBgm(BgmID id, int index = 0);
	// BGMを一時停止する
	void PauseBgm(BgmID id, int index = 0);
	// BGMを再開する
	void ResumeBgm(BgmID id, int index = 0);
	// BGMをフェードさせる
	void FadeBgm(BgmID id, int index, float time, float startVolume, float endVolume);
	// 全ての音声を止める
	void StopAll();
	// 破棄予定のSoundEffectを管理させる
	void RegisterUsingSoundEffect(std::unique_ptr<DirectX::SoundEffect> soundEffect);
	// 全ての効果音を破棄する
	void DestroyAllSounds();

private:
	// BGMのフェードアウトに関する情報
	struct FadeInfo {
		// BGMID
		BgmID bgmId;
		// BGMインデックス
		int   index;
		// 初期ボリューム
		float startVolume;
		// 最終ボリューム
		float endVolume;
		// フェードが完了するまでの時間
		float fadeTime;
		// 経過時間
		float timer;
	};

private:
	// オーディオエンジン
	std::unique_ptr<DirectX::AudioEngine>                      m_audioEngine;
	// 再生中の効果音リスト
	std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_playingSounds;
	// 破棄予定のSoundEffect
	std::vector<std::unique_ptr<DirectX::SoundEffect>>         m_usingSoundEffects;
	// フェードアウトしているBGMリスト
	std::vector<FadeInfo>                                      m_fadeoutBgm;
};


#endif // !AUDIO_MANAGER_DEFINED
