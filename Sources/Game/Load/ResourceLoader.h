#pragma once
#ifndef RESOURCE_LOADER_DEFINED
#define RESOURCE_LOADER_DEFINED


/// <summary>
/// ���\�[�X�ǂݍ���ID
/// </summary>
enum class ResourceLoaderID {
	Common,
	LogoScene,
	TitleScene,
	CharaSelectScene,
	PlayScene,
};

/// <summary>
/// ���\�[�X�ǂݍ��݃N���X
/// </summary>
class ResourceLoader {
public:
	// ���\�[�X��ǂݍ���
	static void Load(ResourceLoaderID id);

	// ���\�[�X���������
	static void Release(ResourceLoaderID id);

private:
	// �������\�[�X���J������
	static void ReleaseAudio();
};


#endif // !RESOURCE_LOADER_DEFINED