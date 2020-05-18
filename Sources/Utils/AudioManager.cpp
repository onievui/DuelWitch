#include "AudioManager.h"
#include "ErrorMessage.h"
#include "ServiceLocater.h"
#include "ResourceManager.h"
#include "MathUtils.h"


/// <summary>
/// コンストラクタ
/// </summary>
AudioManager::AudioManager() {
	// オーディオエンジンを生成する
	DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	flags = flags | DirectX::AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<DirectX::AudioEngine>(flags);

	m_playingSounds.clear();
	m_usingSoundEffects.clear();
}

/// <summary>
/// デストラクタ
/// </summary>
AudioManager::~AudioManager() {
	DestroyAllSounds();
}

/// <summary>
/// オーディオエンジンを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void AudioManager::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	if(!m_audioEngine->Update()) {
		// オーディオデバイスがアクティブでない場合
		if(m_audioEngine->IsCriticalError()) {
			ErrorMessage(L"オーディオデバイスがアクティブでありません");
		}
	}

	// BGMをフェードさせる
	for (std::vector<FadeInfo>::iterator itr = m_fadeoutBgm.begin(); itr != m_fadeoutBgm.end(); ++itr) {
		// タイマーを進める
		itr->timer = std::min(itr->timer + elapsed_time, itr->fadeTime);
		// ボリュームを更新する
		DirectX::SoundEffectInstance* instance = ServiceLocater<ResourceManager<BgmResource>>::Get()
			->GetRawResource(itr->bgmId)->GetInstance(itr->index);
		float volume = Math::Lerp(itr->startVolume, itr->endVolume, itr->timer / itr->fadeTime);
		instance->SetVolume(volume);
		// フェード終了時にボリュームが0なら、BGMを停止させる
		if (itr->timer >= itr->fadeTime && volume <= 0.0f) {
			instance->Stop();
		}
	}

	// フェードが終わったBGM情報を破棄する
	std::vector<FadeInfo>::iterator fadeout_result = std::remove_if(m_fadeoutBgm.begin(), m_fadeoutBgm.end(),
		[](const FadeInfo& info) {
		return (info.timer >= info.fadeTime);
	});
	m_fadeoutBgm.erase(fadeout_result, m_fadeoutBgm.end());


	// 再生が終了した効果音を破棄する
	std::vector<std::unique_ptr<DirectX::SoundEffectInstance>>::iterator result = std::remove_if(m_playingSounds.begin(), m_playingSounds.end(),
		[](const std::unique_ptr<DirectX::SoundEffectInstance>& instance) {
		return (instance->GetState() == DirectX::SoundState::STOPPED);
	});
	m_playingSounds.erase(result, m_playingSounds.end());

	// 使われなくなったSoundEffectを破棄する
	std::vector<std::unique_ptr<DirectX::SoundEffect>>::iterator result2 = std::remove_if(m_usingSoundEffects.begin(), m_usingSoundEffects.end(),
		[](const std::unique_ptr<DirectX::SoundEffect>& effect) {
		return (!effect->IsInUse());
	});
	m_usingSoundEffects.erase(result2, m_usingSoundEffects.end());
}

/// <summary>
/// サウンドを再生する
/// </summary>
/// <param name="id">サウンドID</param>
/// <param name="index">インデックス</param>
void AudioManager::PlaySound(SoundID id, int index) {
	if (!ID_RANGE_CHECK(SoundID, id)) {
		ErrorMessage(L"サウンドの再生で不正なIDが渡されました");
	}

	SoundResource* sound = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawResource(id);
	if (!sound->IsValid(index)) {
		return;
	}
	
	// インスタンスを配列に登録して管理する
	std::unique_ptr<DirectX::SoundEffectInstance> instance = sound->GetResource(index)->CreateInstance();
	instance->Play();
	m_playingSounds.emplace_back(std::move(instance));

	//sound_effect_instance->Play();
	//// One-shot
	//sound->GetResource(index)->Play();
}

/// <summary>
/// BGMを再生する
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">インデックス</param>
void AudioManager::PlayBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGMの再生で不正なIDが渡されました");
	}

	BgmResource* bgm = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawResource(id);
	if (!bgm->IsValid(index)) {
		return;
	}
	DirectX::SoundEffectInstance* sound_effect_instance = bgm->GetInstance(index);
	if (sound_effect_instance) {
		sound_effect_instance->Play(true);
	}
}

/// <summary>
/// サウンドの再生を止める
/// </summary>
/// <param name="id">サウンドiD</param>
/// <param name="index">インデックス</param>
void AudioManager::StopSound(SoundID id, int index) {
	if (!ID_RANGE_CHECK(SoundID, id)) {
		ErrorMessage(L"サウンドの再生の停止で不正なIDが渡されました");
	}

	SoundResource* sound = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawResource(id);
	if (!sound->IsValid(index)) {
		return;
	}
	DirectX::SoundEffectInstance* sound_effect_instance = sound->GetInstance(index);
	if (sound_effect_instance) {
		sound_effect_instance->Stop();
	}
}

/// <summary>
/// BGMの再生を止める
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">インデックス</param>
void AudioManager::StopBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGMの再生の停止で不正なIDが渡されました");
	}

	BgmResource* bgm = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawResource(id);
	if (!bgm->IsValid(index)) {
		return;
	}
	DirectX::SoundEffectInstance* sound_effect_instance = bgm->GetInstance(index);
	if (sound_effect_instance) {
		sound_effect_instance->Stop();
	}
}

/// <summary>
/// BGMを一時停止する
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">インデックス</param>
void AudioManager::PauseBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGMの一時停止で不正なIDが渡されました");
	}

	BgmResource* bgm = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawResource(id);
	if (!bgm->IsValid(index)) {
		return;
	}
	DirectX::SoundEffectInstance* sound_effect_instance = bgm->GetInstance(index);
	if (sound_effect_instance) {
		sound_effect_instance->Pause();
	}
}

/// <summary>
/// BGMを再開する
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">インデックス</param>
void AudioManager::ResumeBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGMの再開で不正なIDが渡されました");
	}

	BgmResource* bgm = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawResource(id);
	if (!bgm->IsValid()) {
		return;
	}
	DirectX::SoundEffectInstance* sound_effect_instance = bgm->GetInstance(index);
	if (sound_effect_instance) {
		sound_effect_instance->Resume();
	}
}

/// <summary>
/// BGMをフェードアウト
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">インデックス</param>
/// <param name="time">フェードが完了するまでの時間</param>
/// <param name="time">フェードが完了するまでの時間</param>
/// <param name="time">フェードが完了するまでの時間</param>
void AudioManager::FadeBgm(BgmID id, int index, float time, float startVolume, float endVolume) {
	FadeInfo fade_info;
	fade_info.bgmId = id;
	fade_info.index = index;
	fade_info.startVolume = startVolume;
	fade_info.endVolume = endVolume;
	fade_info.fadeTime = time;
	fade_info.timer = 0;
	m_fadeoutBgm.push_back(fade_info);
}

/// <summary>
/// 全てのオーディオを止める
/// </summary>
void AudioManager::StopAll() {
	// 全てのサウンドを止める
	for (std::vector<std::unique_ptr<DirectX::SoundEffectInstance>>::iterator itr = m_playingSounds.begin(); itr != m_playingSounds.end(); ++itr) {
		if (*itr) {
			(*itr)->Stop();
		}
	}

	// 全てのBGMを止める
	std::vector<std::unique_ptr<BgmResource>>& bgms = ServiceLocater<ResourceManager<BgmResource>>::Get()->GetRawAllResources();
	for (std::vector<std::unique_ptr<BgmResource>>::iterator itr = bgms.begin(); itr != bgms.end(); ++itr) {
		int num = (*itr)->GetAllResources().size();
		for (int i = 0; i < num; ++i) {
			if ((*itr)->IsValid(i)) {
				(*itr)->GetInstance(i)->Stop();
			}
		}
	}
}

/// <summary>
/// 破棄予定のSoundEffectを管理させる
/// </summary>
/// <param name="soundEffect">SoundEffect</param>
void AudioManager::RegisterUsingSoundEffect(std::unique_ptr<DirectX::SoundEffect> soundEffect) {
	m_usingSoundEffects.emplace_back(std::move(soundEffect));
}

/// <summary>
/// 全ての効果音を破棄する
/// </summary>
void AudioManager::DestroyAllSounds() {
	m_playingSounds.clear();
	m_usingSoundEffects.clear();
}
