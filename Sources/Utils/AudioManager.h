#pragma once
#ifndef AUDIO_MANAGER_DEFINED
#define AUDIO_MANAGER_DEFINED


#include "ResourceID.h"


/// <summary>
/// �I�[�f�B�I�}�l�[�W���N���X
/// </summary>
class AudioManager {
public:
	// �R���X�g���N�^
	AudioManager();

public:
	// �I�[�f�B�I�G���W�����擾����
	DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine.get(); }
	// �I�[�f�B�I���X�V����
	void Update();
	// �T�E���h���Đ�����
	void PlaySound(SoundID id, int index = 0);
	// BGM���Đ�����
	void PlayBgm(BgmID id, int index = 0);
	// �T�E���h�̍Đ����~�߂�
	void StopSound(SoundID id, int index = 0);
	// BGM�̍Đ����~�߂�
	void StopBgm(BgmID id, int index = 0);
	// BGM���ꎞ��~����
	void PauseBgm(BgmID id, int index = 0);
	// BGM���ĊJ����
	void ResumeBgm(BgmID id, int index = 0);
	// �S�Ẳ������~�߂�
	void StopAll();

private:
	// �I�[�f�B�I�G���W��
	std::unique_ptr<DirectX::AudioEngine>              m_audioEngine;

};


#endif // !AUDIO_MANAGER_DEFINED
