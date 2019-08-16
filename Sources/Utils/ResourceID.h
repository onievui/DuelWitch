#pragma once
#ifndef RESOURCE_ID_DEFINED
#define RESOURCE_ID_DEFINED


/// <summary>
/// テクスチャID
/// </summary>
enum class TextureID {
	FireMagicIcon,
	FreezeMagicIcon,
	ThunderMagicIcon,
	// 現状ここから下に書く！
	Num
};

/// <summary>
/// ジオメトリックプリミティブID
/// </summary>
enum class GeometricPrimitiveID {
	FireMagic,
	FreezeMagic,
	ThunderMagic,
	ThunderStrikeMagic,
	Num
};

/// <summary>
/// サウンドID
/// </summary>
enum class SoundID {
	NullData,
	Num
};

/// <summary>
/// BGMID
/// </summary>
enum class BgmID {
	NullData,
	Num
};

/// <summary>
/// 映像ID
/// </summary>
enum class MovieID {
	NullData,
	Num
};

/// <summary>
/// フォントID
/// </summary>
enum class FontID {
	NullData,
	Num
};


#endif // !RESOURCE_ID_DEFINED
