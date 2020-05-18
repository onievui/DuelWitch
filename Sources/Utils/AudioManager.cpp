#include "AudioManager.h"
#include "ErrorMessage.h"
#include "ServiceLocater.h"
#include "ResourceManager.h"
#include "MathUtils.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
AudioManager::AudioManager() {
	// �I�[�f�B�I�G���W���𐶐�����
	DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	flags = flags | DirectX::AudioEngine_Debug;
#endif
	m_audioEngine = std::make_unique<DirectX::AudioEngine>(flags);

	m_playingSounds.clear();
	m_usingSoundEffects.clear();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
AudioManager::~AudioManager() {
	DestroyAllSounds();
}

/// <summary>
/// �I�[�f�B�I�G���W�����X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void AudioManager::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	if(!m_audioEngine->Update()) {
		// �I�[�f�B�I�f�o�C�X���A�N�e�B�u�łȂ��ꍇ
		if(m_audioEngine->IsCriticalError()) {
			ErrorMessage(L"�I�[�f�B�I�f�o�C�X���A�N�e�B�u�ł���܂���");
		}
	}

	// BGM���t�F�[�h������
	for (std::vector<FadeInfo>::iterator itr = m_fadeoutBgm.begin(); itr != m_fadeoutBgm.end(); ++itr) {
		// �^�C�}�[��i�߂�
		itr->timer = std::min(itr->timer + elapsed_time, itr->fadeTime);
		// �{�����[�����X�V����
		DirectX::SoundEffectInstance* instance = ServiceLocater<ResourceManager<BgmResource>>::Get()
			->GetRawResource(itr->bgmId)->GetInstance(itr->index);
		float volume = Math::Lerp(itr->startVolume, itr->endVolume, itr->timer / itr->fadeTime);
		instance->SetVolume(volume);
		// �t�F�[�h�I�����Ƀ{�����[����0�Ȃ�ABGM���~������
		if (itr->timer >= itr->fadeTime && volume <= 0.0f) {
			instance->Stop();
		}
	}

	// �t�F�[�h���I�����BGM����j������
	std::vector<FadeInfo>::iterator fadeout_result = std::remove_if(m_fadeoutBgm.begin(), m_fadeoutBgm.end(),
		[](const FadeInfo& info) {
		return (info.timer >= info.fadeTime);
	});
	m_fadeoutBgm.erase(fadeout_result, m_fadeoutBgm.end());


	// �Đ����I���������ʉ���j������
	std::vector<std::unique_ptr<DirectX::SoundEffectInstance>>::iterator result = std::remove_if(m_playingSounds.begin(), m_playingSounds.end(),
		[](const std::unique_ptr<DirectX::SoundEffectInstance>& instance) {
		return (instance->GetState() == DirectX::SoundState::STOPPED);
	});
	m_playingSounds.erase(result, m_playingSounds.end());

	// �g���Ȃ��Ȃ���SoundEffect��j������
	std::vector<std::unique_ptr<DirectX::SoundEffect>>::iterator result2 = std::remove_if(m_usingSoundEffects.begin(), m_usingSoundEffects.end(),
		[](const std::unique_ptr<DirectX::SoundEffect>& effect) {
		return (!effect->IsInUse());
	});
	m_usingSoundEffects.erase(result2, m_usingSoundEffects.end());
}

/// <summary>
/// �T�E���h���Đ�����
/// </summary>
/// <param name="id">�T�E���hID</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::PlaySound(SoundID id, int index) {
	if (!ID_RANGE_CHECK(SoundID, id)) {
		ErrorMessage(L"�T�E���h�̍Đ��ŕs����ID���n����܂���");
	}

	SoundResource* sound = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawResource(id);
	if (!sound->IsValid(index)) {
		return;
	}
	
	// �C���X�^���X��z��ɓo�^���ĊǗ�����
	std::unique_ptr<DirectX::SoundEffectInstance> instance = sound->GetResource(index)->CreateInstance();
	instance->Play();
	m_playingSounds.emplace_back(std::move(instance));

	//sound_effect_instance->Play();
	//// One-shot
	//sound->GetResource(index)->Play();
}

/// <summary>
/// BGM���Đ�����
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::PlayBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGM�̍Đ��ŕs����ID���n����܂���");
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
/// �T�E���h�̍Đ����~�߂�
/// </summary>
/// <param name="id">�T�E���hiD</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::StopSound(SoundID id, int index) {
	if (!ID_RANGE_CHECK(SoundID, id)) {
		ErrorMessage(L"�T�E���h�̍Đ��̒�~�ŕs����ID���n����܂���");
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
/// BGM�̍Đ����~�߂�
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::StopBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGM�̍Đ��̒�~�ŕs����ID���n����܂���");
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
/// BGM���ꎞ��~����
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::PauseBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGM�̈ꎞ��~�ŕs����ID���n����܂���");
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
/// BGM���ĊJ����
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">�C���f�b�N�X</param>
void AudioManager::ResumeBgm(BgmID id, int index) {
	if (!ID_RANGE_CHECK(BgmID, id)) {
		ErrorMessage(L"BGM�̍ĊJ�ŕs����ID���n����܂���");
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
/// BGM���t�F�[�h�A�E�g
/// </summary>
/// <param name="id">BGMID</param>
/// <param name="index">�C���f�b�N�X</param>
/// <param name="time">�t�F�[�h����������܂ł̎���</param>
/// <param name="time">�t�F�[�h����������܂ł̎���</param>
/// <param name="time">�t�F�[�h����������܂ł̎���</param>
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
/// �S�ẴI�[�f�B�I���~�߂�
/// </summary>
void AudioManager::StopAll() {
	// �S�ẴT�E���h���~�߂�
	for (std::vector<std::unique_ptr<DirectX::SoundEffectInstance>>::iterator itr = m_playingSounds.begin(); itr != m_playingSounds.end(); ++itr) {
		if (*itr) {
			(*itr)->Stop();
		}
	}

	// �S�Ă�BGM���~�߂�
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
/// �j���\���SoundEffect���Ǘ�������
/// </summary>
/// <param name="soundEffect">SoundEffect</param>
void AudioManager::RegisterUsingSoundEffect(std::unique_ptr<DirectX::SoundEffect> soundEffect) {
	m_usingSoundEffects.emplace_back(std::move(soundEffect));
}

/// <summary>
/// �S�Ă̌��ʉ���j������
/// </summary>
void AudioManager::DestroyAllSounds() {
	m_playingSounds.clear();
	m_usingSoundEffects.clear();
}
