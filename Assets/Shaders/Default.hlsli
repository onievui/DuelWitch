
struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};


float gray(float4 rgba) {
	return rgba.r*0.2f + rgba.g*0.7f + rgba.b*0.1f;
}

float pow2r(float t) {
	return t * (2.0f - t);
}

float pow3r(float t) {
	return t * t*(3.0f - 2.0f*t);
}

// 0~1‚ð•Ô‚·
float random(float2 uv) {
	return frac(sin(dot(uv, float2(12.9898f, 78.233f)))* 43758.5453f);
}

// -‚P~1‚ð•Ô‚· 
float2 random2(float2 uv) {
	float2 r = float2(dot(uv, float2(127.1f, 311.7f)), dot(uv, float2(269.5f, 183.3f)));
	return -1.0 + 2.0f * frac(sin(r)*43758.5453123f);
}

float noise(float2 uv) {
	float2 p = floor(uv);
	return random(p);
}

float perlinNoise(float2 uv) {
	float2 p = floor(uv);
	float2 f = frac(uv);
	float2 u = f * f * (3.0f - 2.0f * f);

	float v00 = random2(p + float2(0, 0)).x;
	float v10 = random2(p + float2(1, 0)).x;
	float v01 = random2(p + float2(0, 1)).x;
	float v11 = random2(p + float2(1, 1)).x;

	return lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
		u.y) + 0.5f;
}

float fBm(float2 uv) {
	float f = 0;
	float2 q = uv;

	f += 0.5000f*perlinNoise(q); q = q * 2.01f;
	f += 0.2500f*perlinNoise(q); q = q * 2.02f;
	//f += 0.1250f*perlinNoise(q); q = q * 2.03f; 
	f += 0.0625f*perlinNoise(q); q = q * 2.01f;

	return f;
}

float4 blur(float2 uv, Texture2D t, SamplerState samplerState) {
	float b = 0.06f;
	float c = 1.0f - b * 8;
	float4 blur_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 dest = t.Sample(samplerState, uv + float2(0.0f, 0.0f)) *c
		+ t.Sample(samplerState, uv + float2(-0.01f, -0.01f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(-0.01f, 0.0f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(-0.01f, 0.01f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(0.0f, -0.01f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(0.0f, 0.01f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(0.01f, -0.01f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(0.01f, 0.0f))* b*blur_color
		+ t.Sample(samplerState, uv + float2(0.01f, 0.01f))* b*blur_color;
	return dest;
}

float2 mosaic(float2 baseUV, float mag) {
	return floor(baseUV * mag) / mag;
}




static const float PI = 3.14159265f;
static const float PI2 = PI * 2;