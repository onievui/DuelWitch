#pragma once
#ifndef RESOURCE_ID_DEFINED
#define RESOURCE_ID_DEFINED


/*
/// <summary>
/// サンプルID
/// </summary>
enum class SampleID {
	SampleResource1,	// = 0
	SampleResource2,	// = 1
	SampleResource3,
	Num	// 末尾は"Num"
};
*/


/// <summary>
/// テクスチャID
/// </summary>
enum class TextureID {
	Logo,
	Title,
	UIFrame,
	CharaSelectBackGround,
	CharaIcon,
	CharaSelectMarker,
	MagicIcon,
	MagicAiming,
	HpBar,
	SpBar,
	BlackBack,

	Particle,
	FieldShield,
	FireMagicEffect,

	Num
};

/// <summary>
/// ジオメトリックプリミティブID
/// </summary>
enum class GeometricPrimitiveID {
	Element,
	NormalMagic,
	FireMagic,
	FreezeMagic,
	ThunderMagic,
	ThunderStrikeMagic,
	Num
};

/// <summary>
/// モデルID
/// </summary>
enum class ModelID {
	Bloom,
	Skydome,

	Num,
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
/// フォントID
/// </summary>
enum class FontID {
	Default,
	Num
};

/// <summary>
/// 頂点シェーダID
/// </summary>
enum class VertexShaderID {
	Default,
	Num
};

/// <summary>
/// ジオメトリシェーダID
/// </summary>
enum class GeometryShaderID {
	Billboard,
	Num
};

/// <summary>
/// ピクセルシェーダID
/// </summary>
enum class PixelShaderID {
	Default,
	Num
};

#endif // !RESOURCE_ID_DEFINED
