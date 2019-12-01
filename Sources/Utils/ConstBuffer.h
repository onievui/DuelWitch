#pragma once
#ifndef CONST_BUFFER_DEFIND
#define CONST_BUFFER_DEFIND


/// <summary>
/// �r���{�[�h�p�萔�o�b�t�@
/// </summary>
struct BillboardBuffer {
	DirectX::SimpleMath::Matrix		matWorld;
	DirectX::SimpleMath::Matrix		matView;
	DirectX::SimpleMath::Matrix		matProj;
	DirectX::SimpleMath::Vector4	eye;
	DirectX::SimpleMath::Vector4	diffuse;
	int                             lookAt;
	DirectX::SimpleMath::Vector3    _dummy;
};


/// <summary>
/// �u���[�p�p�����[�^
/// </summary>
struct BlurParam {
	DirectX::SimpleMath::Vector4 offset[16];
	int                          sampleCount;
	DirectX::SimpleMath::Vector3 _dummy;
};


/// <summary>
/// �������@�p�萔�o�b�t�@
/// </summary>
struct ThunderStrikeBuffer {
	BlurParam                    blurParam;
	DirectX::SimpleMath::Vector4 time;
};



#endif // !CONST_BUFFER_DEFIND
