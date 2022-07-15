#include"DefaultShader.hlsli"
Use_ObjectInformation(b1)
Use_Material(b2)
float4 PSMain(Pixel_Normal_Color pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);
	Light.a = 0;

	float4 Color = pixel.color;
	return  Color;
}
