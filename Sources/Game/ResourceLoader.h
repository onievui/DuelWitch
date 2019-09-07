#pragma once
#ifndef RESOURCE_LOADER_DEFINED
#define RESOURCE_LOADER_DEFINED


/// <summary>
/// ���\�[�X�ǂݍ���ID
/// </summary>
enum class ResourceLoaderID {
	Common,
	LogoScene,
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
};


#endif // !RESOURCE_LOADER_DEFINED