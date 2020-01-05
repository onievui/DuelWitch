#include "AudioManager.h"
#include "ErrorMessage.h"
#include "ServiceLocater.h"
#include "ResourceManager.h"


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
}

/// <summary>
/// �I�[�f�B�I�G���W�����X�V����
/// </summary>
void AudioManager::Update() {
	if(!m_audioEngine->Update()) {
		// �I�[�f�B�I�f�o�C�X���A�N�e�B�u�łȂ��ꍇ
		if(m_audioEngine->IsCriticalError()) {
			ErrorMessage(L"�I�[�f�B�I�f�o�C�X���A�N�e�B�u�ł���܂���");
		}
	}
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
	// One-shot
	sound->GetResource(index)->Play();
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
/// �S�ẴI�[�f�B�I���~�߂�
/// </summary>
void AudioManager::StopAll() {
	// �S�ẴT�E���h���~�߂�
	std::vector<std::unique_ptr<SoundResource>>& sounds = ServiceLocater<ResourceManager<SoundResource>>::Get()->GetRawAllResources();
	for (std::vector<std::unique_ptr<SoundResource>>::iterator itr = sounds.begin(); itr != sounds.end(); ++itr) {
		int num = (*itr)->GetAllResources().size();
		for (int i = 0; i < num; ++i) {
			if ((*itr)->IsValid(i)) {
				(*itr)->GetInstance(i)->Stop();
			}
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
