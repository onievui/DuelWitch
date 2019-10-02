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
	DirectX::SimpleMath::Vector4	Diffuse;
	int                lookAt;
	DirectX::SimpleMath::Vector3 _dummy;
};


#endif // !CONST_BUFFER_DEFIND
