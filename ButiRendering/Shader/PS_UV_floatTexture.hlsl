#include"DefaultShader.hlsli"
Use_ObjectInformation(b1)
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float sampled = floatTexture.Sample(mainSampler,pixel.uv);
	float4 Tex = float4(1.0,1.0,1.0, sampled)*color;
	clip(Tex.a < 0.1f ? -1 : 1);
	return Tex;
}
