#include "AudioManager.h"
#include "ErrorMessage.h"
#include "ServiceLocater.h"
#include "ResourceManager.h"


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
void AudioManager::Update() {
	if(!m_audioEngine->Update()) {
		// オーディオデバイスがアクティブでない場合
		if(m_audioEngine->IsCriticalError()) {
			ErrorMessage(L"オーディオデバイスがアクティブでありません");
		}
	}

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
	//DirectX::SoundEffectInstance* sound_effect_instance = sound->GetInstance(index);
	//if (!sound_effect_instance) {
	//	return;
	//}
	//if (sound_effect_instance->GetState() != DirectX::SoundState::STOPPED) {
	//	sound_effect_instance->Stop();
	//}

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
/// 全てのオーディオを止める
/// </summary>
void AudioManager::StopAll() {
	// 全てのサウンドを止める
	//std::vector<std::unique_ptr<SoundResource>>& sounds = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawAllResources();
	//for (std::vector<std::unique_ptr<SoundResource>>::iterator itr = sounds.begin(); itr != sounds.end(); ++itr) {
	//	int num = (*itr)->GetAllResources().size();
	//	for (int i = 0; i < num; ++i) {
	//		if ((*itr)->IsValid(i)) {
	//			(*itr)->GetInstance(i)->Stop();
	//		}
	//	}
	//}

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
