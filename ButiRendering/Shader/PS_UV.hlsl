#include"DefaultShader.hlsli"
Use_ObjectInformation(b1)
float4 PSMain(Pixel_UV pixel) : SV_TARGET
{
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv))*color;
	clip(Tex.a < 0.1f ? -1 : 1);
	return Tex;
}
