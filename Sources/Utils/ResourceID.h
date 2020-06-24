#pragma once
#ifndef RESOURCE_ID_DEFINED
#define RESOURCE_ID_DEFINED


#define ID_RANGE_CHECK(IDType, id) ((id >= static_cast<IDType>(0) && id < IDType::Num))


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
	CharaManual,
	CharaManualText,
	CharaPortrait,
	CharaPortrait2,
	CharaSelectMarker,
	CharaIcon,
	MagicIcon,
	MagicAiming,
	HpBar,
	SpBar,
	Fade,
	YouLose,
	Player1Win,

	Particle,
	PlayerCharge,

	FieldShield,

	Hit,

	FireMagicEffect,
	Ice,
	ThunderStrikeMagicEffect,
	EffectiveEffect,

	BoostEffect,

	EventLogFrame,
	EventLog,

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
	Chara,
	Bloom,
	Skydome,

	Num,
};

/// <summary>
/// サウンドID
/// </summary>
enum class SoundID {
	Cursor,
	Decision,
	Ready,
	Pause,

	NormalMagic,
	FireMagic,
	FreezeMagic,
	ThunderMagic,
	ThunderStrikeMagic,
	Effective,
	Charge,
	Damage,
	Boost,
	Rolling,
	GetElement,

	Num
};

/// <summary>
/// BGMID
/// </summary>
enum class BgmID {
	Title,
	CharaSelect,
	Battle,
	Win,
	Lose,
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
	Ice,
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
	ThunderStrikeMagic,
	Boost,
	Num
};

#endif // !RESOURCE_ID_DEFINED
