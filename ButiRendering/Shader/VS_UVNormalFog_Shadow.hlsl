#include"DefaultShader.hlsli"
Use_ObjectMatrix(b0)
Use_ObjectInformation(b1)
Use_RendererStatus(b3)
Pixel_UV_Normal_Fog_Shadow VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Fog_Shadow output;
	output.position = mul(vertex.position, mvpMatrix);
	float dist = length(mul( vertex.position,modelMatrix )- cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	
	output.normal =  mul(vertex.normal, (float3x3)modelMatrix);

	output.uv = vertex.uv;


	float4 Pos = mul(vertex.position, modelMatrix);
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xyz = Pos.xyz / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) / 2.0f;
	output.shadowPos.y = (1.0f - Pos.y) / 2.0f;
	output.shadowPos.z = Pos.z;
	return output;
}