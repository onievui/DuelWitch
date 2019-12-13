#pragma once
#ifndef INPUT_ELEMENT_DESC_DEFINED
#define INPUT_ELEMENT_DESC_DEFINED


#include "ResourceID.h"
#include "ErrorMessage.h"


/// <summary>
/// ���̓��C�A�E�g�̏��N���X
/// </summary>
class InputElementDesc {
public:
	// ���̓��C�A�E�g�̏��𐶐�����
	static std::vector<D3D11_INPUT_ELEMENT_DESC> Create(VertexShaderID id) {
		// �x�N�g���̃T�C�Y���`���Ă���
		constexpr size_t vector3_size = sizeof(DirectX::SimpleMath::Vector3);
		constexpr size_t vector4_size = sizeof(DirectX::SimpleMath::Vector4);

		switch (id) {
		case VertexShaderID::Default:
			return std::vector<D3D11_INPUT_ELEMENT_DESC> 
			{
				{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                           D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vector3_size,                D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,       0, vector3_size + vector4_size, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			break;
		case VertexShaderID::Ice:
			return std::vector<D3D11_INPUT_ELEMENT_DESC>
			{
				{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                           D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   	0, DXGI_FORMAT_R32G32B32_FLOAT,    0, vector3_size,                D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,       0, vector3_size*2,              D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			break;
		default:
			ErrorMessage(L"���̓��C�A�E�g�̍쐬�ŕs����ID���n����܂���");
			break;
		}
		return std::vector<D3D11_INPUT_ELEMENT_DESC>();
	}
};


#endif // !INPUT_ELEMENT_DESC_DEFINED
