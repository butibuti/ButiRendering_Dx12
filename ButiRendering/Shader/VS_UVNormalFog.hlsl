#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_ObjectInformation(b1)
Use_RendererStatus(b3)
Pixel_UV_Normal_Fog VSMain(Vertex_UV_Normal vertex)
{
	Pixel_UV_Normal_Fog output;
	output.position = mul(vertex.position, mvpMatrix);
	float dist = length(mul( vertex.position,modelMatrix )- cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	
	output.normal =  mul(vertex.normal, (float3x3)modelMatrix);

	output.uv = vertex.uv;
	return output;
}