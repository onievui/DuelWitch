#include "Particle.hlsli"
static const int vnum = 4;

static const float4 offset_array[vnum] =
{
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
	//mat._14 = pos.x;
	//mat._24 = pos.y;
	//mat._34 = pos.z;
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
	return mat;
}

matrix CreateBillboard(float3 pos, float3 eye, float3 up) {
	//XMVECTOR O = XMLoadFloat3(&object);
	//XMVECTOR C = XMLoadFloat3(&cameraPosition);
	//XMVECTOR Z = XMVectorSubtract(O, C);

	//float3 z = billboardCenter - camera;
	//float3 z = eye;
	float3 z = normalize(eye);

	//XMVECTOR N = XMVector3LengthSq(Z);
	//if (XMVector3Less(N, g_XMEpsilon))
	//{
	//	if (cameraForward)
	//	{
	//		XMVECTOR F = XMLoadFloat3(cameraForward);
	//		Z = XMVectorNegate(F);
	//	}
	//	else
	//		Z = g_XMNegIdentityR2;
	//}
	//else
	//{
	//	Z = XMVector3Normalize(Z);
	//}

	//XMVECTOR up = XMLoadFloat3(&cameraUp);
	//XMVECTOR X = XMVector3Cross(up, Z);
	//X = XMVector3Normalize(X);
	//
	//XMVECTOR Y = XMVector3Cross(Z, X);

	float3 x = cross(up, z);
	x = normalize(x);
	float3 y = cross(z, x);

	//XMMATRIX M;
	//M.r[0] = X;
	//M.r[1] = Y;
	//M.r[2] = Z;
	//M.r[3] = XMVectorSetW(O, 1.f);

	matrix mat;
	mat._11 = x.x; mat._12 = x.y; mat._13 = x.z; mat._14 = 0;
	mat._21 = y.x; mat._22 = y.y; mat._23 = y.z; mat._24 = 0;
	mat._31 = z.x; mat._32 = z.y; mat._33 = z.z; mat._34 = 0;
	mat._41 = 0; mat._42 = 0; mat._43 = 0; mat._44 = 1;

	return mul(mat, CreateTranslate(pos));

	//Matrix R;
	//XMStoreFloat4x4(&R, M);
	//return R;
}

[maxvertexcount(vnum*2)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	//matrix mat = matWorld;
	//mat._11 *= 1.1f;
	//mat._22 *= 1.1f;
	//mat._33 *= 1.1f;

	//for (int i = vnum - 1; i >= 0; i--)
	//{
	//	PS_INPUT element;

	//	element.Pos = input[0].Pos + offset_array[i];
	//	element.Pos = mul(element.Pos, mat);
	//	element.Pos = mul(element.Pos, matView);
	//	element.Pos = mul(element.Pos, matProj);

	//	element.Color = float4(0, 0, 0, 1);

	//	element.Tex = -offset_array[i] + float2(1.0f, 1.0f);
	//	element.Tex = element.Tex * 0.5f;
	//	output.Append(element);
	//	{
	PS_INPUT element;

			//const int offset[vnum] = {
			//	0,2,4,1,3,5
	//}
	for (int n = 0; n < 2; n++) {
		for (int i = 0; i < vnum; i++) {
			//};

			//float angle = -PI2*offset[i] / vnum+PI/2;

			matrix mat;
			//mat = mul(CreateTranslate(input[0].Pos.xyz), matWorld);
			//mat = mul(inv, CreateTranslate(input[0].Pos.xyz));
			//mat = mul(matWorld, CreateTranslate(input[0].Pos.xyz));
			//mat = mul(matViewInv, CreateTranslate(-(float3(matWorld._41, matWorld._42, matWorld._43)+input[0].Pos.xyz)+camera.xyz));
			//mat = CreateBillboard(mul(input[0].Pos.xyz, matWorld), camera, float3(0, 1, 0));
			float3 center = float3(matWorld._41, matWorld._42, matWorld._43);
			float4 world_pos = mul(input[0].Pos, matWorld);
			mat = CreateBillboard(world_pos.xyz, eye, float3(0, 1, 0));
			float4 pos = float4(0,0,0,1);
			if (n == 0) {
				//element.Pos = input[0].Pos + offset_array[i] * 0.5f + float4(cos(time*PI / 2), 0, sin(time*PI / 2), 0)*0;
				//element.Pos = mul(element.Pos, mat);
				element.Pos = pos + offset_array[i] * 0.5f;
				element.Pos = mul(element.Pos, mat);

			}
			else {
				//element.Pos = input[0].Pos + offset_array[i] * 0.5f + float4(0, sin(time*PI / 2), cos(time*PI / 2), 0);
				//element.Pos = mul(element.Pos, matWorld);
				element.Pos = pos + offset_array[i] * 0.5f + float4(0, sin(time*PI / 2), cos(time*PI / 2), 0);
				element.Pos = mul(element.Pos, mat);
			}
			

			element.Pos = mul(element.Pos, matView);
			element.Pos = mul(element.Pos, matProj);

			//element.Color = input[0].Color;
			element.Color = float4(1, 1, 1, 1);

			//element.Tex = float2(0, 0);

			element.Tex = -offset_array[i].xy + float2(1.0f, 1.0f);
			element.Tex = element.Tex * 0.5f;
			output.Append(element);
			//if (i == 2) {
			//	output.RestartStrip();
			//}
		}
		output.RestartStrip();
	}

}