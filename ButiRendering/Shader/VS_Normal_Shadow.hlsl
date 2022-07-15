#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_RendererStatus(b3)
Pixel_Normal_Shadow VSMain(Vertex_Normal vertex)
{
	Pixel_Normal_Shadow output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal =  mul(vertex.normal, (float3x3)modelMatrix);

	float4 Pos = mul(vertex.position, modelMatrix);
	Pos = mul(Pos, shadowvpMatrix);
	Pos.xyz = Pos.xyz / Pos.w;
	output.shadowPos.x = (1.0f + Pos.x) / 2.0f;
	output.shadowPos.y = (1.0f - Pos.y) / 2.0f;
	output.shadowPos.z = Pos.z;
	return output;
}