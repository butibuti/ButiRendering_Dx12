#include"DefaultShader.hlsli"
Use_ObjectInformation(b1)
Use_Material(b2)
Use_RendererStatus(b3)
float4 PSMain(Pixel_Normal_Fog pixel) : SV_TARGET
{
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * diffuse + emissive);

	Light.rgb += specular.rgb * specular.a;
	Light.a = 0;
	float3 retRGB = lerp(fogColor.rgb, (Light).rgb, pixel.fog);
	return float4(retRGB,1.0);//,0.5f);
}