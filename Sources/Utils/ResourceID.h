#pragma once
#ifndef RESOURCE_ID_DEFINED
#define RESOURCE_ID_DEFINED


/*
/// <summary>
/// �T���v��ID
/// </summary>
enum class SampleID {
	SampleResource1,	// = 0
	SampleResource2,	// = 1
	SampleResource3,
	Num	// ������"Num"
};
*/


/// <summary>
/// �e�N�X�`��ID
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
/// �W�I���g���b�N�v���~�e�B�uID
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
/// ���f��ID
/// </summary>
enum class ModelID {
	Bloom,
	Skydome,

	Num,
};

/// <summary>
/// �T�E���hID
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
/// �t�H���gID
/// </summary>
enum class FontID {
	Default,
	Num
};

/// <summary>
/// ���_�V�F�[�_ID
/// </summary>
enum class VertexShaderID {
	Default,
	Num
};

/// <summary>
/// �W�I���g���V�F�[�_ID
/// </summary>
enum class GeometryShaderID {
	Billboard,
	Num
};

/// <summary>
/// �s�N�Z���V�F�[�_ID
/// </summary>
enum class PixelShaderID {
	Default,
	Num
};

#endif // !RESOURCE_ID_DEFINED
