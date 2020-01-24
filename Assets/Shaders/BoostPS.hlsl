#include "Default.hlsli"


cbuffer BoostBuffer : register(b0) {
	float4 time;
};


Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT_SPRITEBATCH input) : SV_TARGET
{
	float4 diff = tex.Sample(samLinear, input.Tex);
	diff *= input.Color;
	float2 uv = input.Tex;

	float angle = atan2(uv.y - 0.5f, uv.x - 0.5f);
	angle += PI;
	angle /= PI * 2;

	float speed = floor(time.x * 60) / 60.0f;

	if (random(floor(angle * 100) + speed) > 0.1f)
	{
		diff.a = 0;
	}
	diff.a *= 0.7f;

	return diff;
}