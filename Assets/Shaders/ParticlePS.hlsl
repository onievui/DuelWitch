#include "Particle.hlsli"


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	float4 diff = tex.Sample(samLinear, input.Tex);
	//float4 diff2 = tex2.Sample(samLinear, input.Tex);
	////diff = float4(input.Color.x,1,input.Color.z, diff.a);
	//
	//if (diff.a > 0) {
	//	diff.rgb = diff2.rgb;
	//}
	//diff.a *= 0.2f;

	diff.rgb = float3(cos(time*PI / 2), 1, sin(time*PI / 2));
	
	return diff*input.Color;

	//return float4(1,1,1,1);
}