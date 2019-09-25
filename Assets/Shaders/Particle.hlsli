
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matViewInv;
	matrix matProj;
	float4 camera;
	float4 eye;
	float4 diffuse;
	float  time;
};

static const float PI = 3.14159265f;
static const float PI2 = PI * 2;