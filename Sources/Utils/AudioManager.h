#pragma once
#ifndef AUDIO_MANAGER_DEFINED
#define AUDIO_MANAGER_DEFINED


#include "ResourceID.h"


/// <summary>
/// オーディオマネージャクラス
/// </summary>
class AudioManager {
public:
	// コンストラクタ
	AudioManager();

public:
	// オーディオエンジンを取得する
	DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	// オーディオを更新する
	void Update();
	// サウンドを再生する
	void PlaySound(SoundID id, int index = 0);
	// BGMを再生する
	void PlayBgm(BgmID id, int index = 0);
	// サウンドの再生を止める
	void StopSound(SoundID id, int index = 0);
	// BGMの再生を止める
	void StopBgm(BgmID id, int index = 0);
	// BGMを一時停止する
	void PauseBgm(BgmID id, int index = 0);
	// BGMを再開する
	void ResumeBgm(BgmID id, int index = 0);
	// 全ての音声を止める
	void StopAll();

private:
	// オーディオエンジン
	std::unique_ptr<DirectX::AudioEngine>              m_audioEngine;

};


#endif // !AUDIO_MANAGER_DEFINED
