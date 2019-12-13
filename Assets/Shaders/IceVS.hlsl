#include "Default.hlsli"


cbuffer IceBuffer : register(b2) {
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 eye;
};


PS_INPUT main(VS_INPUT_NORMAL input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos, 1);
	output.Pos = mul(output.Pos, matWorld);
	output.Pos = mul(output.Pos, matView);
	output.Pos = mul(output.Pos, matProj);

	float4 color = float4(1, 1, 1, 1);
	float alpha = 1 - (abs(dot(eye.xyz, input.Normal)));
	//color.a = alpha * 1.5f;
	color.a = max(alpha * 1.5f, 0.2f);
	output.Color = color;


	output.Tex = input.Tex;

	return output;
}