#include"DefaultShader.hlsli"

Use_ObjectInformation(b1)
Use_Material(b2)
float4 PSMain(Pixel_UV_Normal pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);

	Light.rgb += specular.rgb * specular.a;
	Light.a = 0;
	float4 Tex = (mainTexture.Sample(mainSampler,pixel.uv).rgba)*color;
	

	clip(Tex.a < 0.001f ? -1 : 1);
	return  float4((Tex * Light).rgb, Tex.a * ambient.a);
}