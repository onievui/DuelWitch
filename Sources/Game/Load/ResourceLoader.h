#pragma once
#ifndef RESOURCE_LOADER_DEFINED
#define RESOURCE_LOADER_DEFINED


/// <summary>
/// リソース読み込みID
/// </summary>
enum class ResourceLoaderID {
	Common,
	LogoScene,
	TitleScene,
	CharaSelectScene,
	PlayScene,
};

/// <summary>
/// リソース読み込みクラス
/// </summary>
class ResourceLoader {
public:
	// リソースを読み込む
	static void Load(ResourceLoaderID id);

	// リソースを解放する
	static void Release(ResourceLoaderID id);

private:
	// 音声リソースを開放する
	static void ReleaseAudio();
};


#endif // !RESOURCE_LOADER_DEFINED