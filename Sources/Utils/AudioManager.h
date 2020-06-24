#pragma once
#ifndef AUDIO_MANAGER_DEFINED
#define AUDIO_MANAGER_DEFINED


#include <Framework\StepTimer.h>
#include "ResourceID.h"


/// <summary>
/// �I�[�f�B�I�}�l�[�W���N���X
/// </summary>
class AudioManager {
public:
	// �R���X�g���N�^
	AudioManager();
	// �f�X�g���N�^
	~AudioManager();

public:
	// �I�[�f�B�I�G���W�����擾����
	DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	// �I�[�f�B�I���X�V����
	void Update(const DX::StepTimer& timer);
	// �T�E���h���Đ�����
	void PlaySound(SoundID id, int index = 0, float volume = 1.0f);
	// BGM���Đ�����
	void PlayBgm(BgmID id, int index = 0, float volume = 1.0f);
	// �T�E���h�̍Đ����~�߂�
	void StopSound(SoundID id, int index = 0);
	// BGM�̍Đ����~�߂�
	void StopBgm(BgmID id, int index = 0);
	// BGM���ꎞ��~����
	void PauseBgm(BgmID id, int index = 0);
	// BGM���ĊJ����
	void ResumeBgm(BgmID id, int index = 0);
	// BGM���t�F�[�h������
	void FadeBgm(BgmID id, int index, float time, float startVolume, float endVolume);
	// �S�Ẳ������~�߂�
	void StopAll();
	// �j���\���SoundEffect���Ǘ�������
	void RegisterUsingSoundEffect(std::unique_ptr<DirectX::SoundEffect> soundEffect);
	// �S�Ă̌��ʉ���j������
	void DestroyAllSounds();

private:
	// BGM�̃t�F�[�h�A�E�g�Ɋւ�����
	struct FadeInfo {
		// BGMID
		BgmID bgmId;
		// BGM�C���f�b�N�X
		int   index;
		// �����{�����[��
		float startVolume;
		// �ŏI�{�����[��
		float endVolume;
		// �t�F�[�h����������܂ł̎���
		float fadeTime;
		// �o�ߎ���
		float timer;
	};

private:
	// �I�[�f�B�I�G���W��
	std::unique_ptr<DirectX::AudioEngine>                      m_audioEngine;
	// �Đ����̌��ʉ����X�g
	std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_playingSounds;
	// �j���\���SoundEffect
	std::vector<std::unique_ptr<DirectX::SoundEffect>>         m_usingSoundEffects;
	// �t�F�[�h�A�E�g���Ă���BGM���X�g
	std::vector<FadeInfo>                                      m_fadeoutBgm;
};


#endif // !AUDIO_MANAGER_DEFINED
