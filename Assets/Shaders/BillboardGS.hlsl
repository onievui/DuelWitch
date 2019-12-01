#include "Default.hlsli"


cbuffer BillboardBuffer	: register(b0) {
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 eye;
	float4 diffuse;
	int   lookAt;
	float3 _dummy;
};


static const int vnum = 4;

static const float4 offset_array[vnum] = {
	float4(-1.0f, -1.0f, 0, 0),	// ç∂â∫
	float4(-1.0f, +1.0f, 0, 0),	// ç∂è„
	float4(+1.0f, -1.0f, 0, 0),	// âEè„
	float4(+1.0f, +1.0f, 0, 0),	// âEâ∫

};

matrix CreateTranslate(float3 pos) {
	matrix mat;
	mat._11 = 1; mat._12 = 0; mat._13 = 0; mat._14 = 0;
	mat._21 = 0; mat._22 = 1; mat._23 = 0; mat._24 = 0;
	mat._31 = 0; mat._32 = 0; mat._33 = 1; mat._34 = 0;
	mat._41 = 0; mat._42 = 0; mat._43 = 0; mat._44 = 1;

	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	return mat;
}

matrix CreateRotationZ(float rad) {
	matrix mat;
	mat._11 = 0; mat._12 = 0; mat._13 = 0; mat._14 = 0;
	mat._21 = 0; mat._22 = 0; mat._23 = 0; mat._24 = 0;
	mat._31 = 0; mat._32 = 0; mat._33 = 0; mat._34 = 0;
	mat._41 = 0; mat._42 = 0; mat._43 = 0; mat._44 = 1;

	float sin_rad = sin(rad);
	float cos_rad = cos(rad);

	mat._11 = cos_rad;
	mat._12 = -sin_rad;
	mat._21 = sin_rad;
	mat._22 = cos_rad;
	mat._33 = 1;
	return mat;
}

matrix CreateScale(float3 scale) {
	matrix mat;
	mat._11 = 1; mat._12 = 0; mat._13 = 0; mat._14 = 0;
	mat._21 = 0; mat._22 = 1; mat._23 = 0; mat._24 = 0;
	mat._31 = 0; mat._32 = 0; mat._33 = 1; mat._34 = 0;
	mat._41 = 0; mat._42 = 0; mat._43 = 0; mat._44 = 1;

	mat._11 = scale.x;
	mat._22 = scale.y;
	mat._33 = scale.z;
	return mat;
}

matrix CreateBillboard(float3 pos, float3 eye, float3 up) {
	float3 z;
	if (length(eye) == 0) {
		z = float3(0, 0, -1);
	}
	else {
		z = normalize(eye);
	}

	float3 x = cross(up, z);
	x = normalize(x);
	float3 y = cross(z, x);

	matrix mat;
	mat._11 = x.x; mat._12 = x.y; mat._13 = x.z; mat._14 = 0;
	mat._21 = y.x; mat._22 = y.y; mat._23 = y.z; mat._24 = 0;
	mat._31 = z.x; mat._32 = z.y; mat._33 = z.z; mat._34 = 0;
	mat._41 = 0; mat._42 = 0; mat._43 = 0; mat._44 = 1;

	return mul(mat, CreateTranslate(pos));
}



[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
) 
{
	PS_INPUT element;

	float3 center = float3(matWorld._41, matWorld._42, matWorld._43);
	float4 world_pos = mul(input[0].Pos, matWorld);

	matrix mat = mul(CreateScale(float3(input[0].Tex.x, input[0].Tex.x, input[0].Tex.x)), CreateRotationZ(input[0].Tex.y));
	if (lookAt) {
		mat = mul(mat, CreateBillboard(world_pos.xyz, (world_pos-eye).xyz, float3(0, 1, 0)));
	}
	else {
		mat = mul(mat, CreateBillboard(world_pos.xyz, eye.xyz, float3(0, 1, 0)));
	}
	//mat = mul(mat, CreateBillboard(world_pos.xyz, eye.xyz, float3(0, 1, 0)));

	float4 pos = float4(0, 0, 0, 1);

	for (int i = 0; i < vnum; ++i) {
		element.Pos = pos + offset_array[i] * 0.5f;
		element.Pos = mul(element.Pos, mat);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;

		element.Tex = -offset_array[i].xy + float2(1.0f, 1.0f);
		element.Tex = element.Tex * 0.5f;
		output.Append(element);
	}

}