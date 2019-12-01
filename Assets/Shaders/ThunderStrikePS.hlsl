#include "Default.hlsli"


struct BlurParam {
	float4 offset[16];
	int sampleCount;
	float3 _dummy;
};

cbuffer BlurBuffer	: register(b1) {
	BlurParam blurParam;
	float4 blur_time;
}


float4 gaussBlur(float2 uv, Texture2D t, SamplerState samplerState) {
	float4 result = (float4)0;

	for (int i = 0; i < blurParam.sampleCount; ++i) {
		result += blurParam.offset[i].z * t.Sample(samplerState, uv + blurParam.offset[i].xy);
	}

	return result;
}

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET {
	float2 uv = input.Tex;

	const float speed = 0.5f;

	int index = (int)floor(blur_time.x * 16);
	float rand = random(float2(index, index));

	if (rand < 0.4f) {
		uv.x *= -1;
	}

	float4 diff = gaussBlur(uv, tex, samLinear);

	diff.b *= (0.6f + 0.4f*blur_time.z) * 0 + 0.5f;
	//diff.a *= 0.8f;

	return diff;
}