#include"DefaultShader.hlsli"
Use_Material(b2)
Use_ObjectInformation(b1)
float4 PSMain(Pixel_Normal pixel) : SV_TARGET
{ 
	float3 lightdir = normalize(lightDir.xyz);
	float3 N1 = normalize(pixel.normal);
	float4 Light = saturate(dot(N1, -lightdir) * emissive+ diffuse );

	Light.rgb += specular.rgb * specular.a;
	Light.a = diffuse.a;
	Light *= color;
	clip(Light.a < 0.001f ? -1 : 1);
	return  Light;
}