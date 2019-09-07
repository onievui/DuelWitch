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
	MagicIcon,
	MagicAiming,
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
	BloomModel,
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
/// �f��ID
/// </summary>
enum class MovieID {
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


#endif // !RESOURCE_ID_DEFINED
