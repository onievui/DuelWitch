#pragma once
#ifndef CONST_BUFFER_DEFIND
#define CONST_BUFFER_DEFIND


/// <summary>
/// ビルボード用定数バッファ
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
/// ブラー用パラメータ
/// </summary>
struct BlurParam {
	DirectX::SimpleMath::Vector4 offset[16];
	int                          sampleCount;
	DirectX::SimpleMath::Vector3 _dummy;
};


/// <summary>
/// 落雷魔法用定数バッファ
/// </summary>
struct ThunderStrikeBuffer {
	BlurParam                    blurParam;
	DirectX::SimpleMath::Vector4 time;
};



#endif // !CONST_BUFFER_DEFIND
