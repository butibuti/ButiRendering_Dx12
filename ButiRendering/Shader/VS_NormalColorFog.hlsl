#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_ObjectInformation(b1)
Use_RendererStatus(b3)
Pixel_Normal_Color_Fog VSMain(Vertex_Normal_Color vertex)
{
	Pixel_Normal_Color_Fog output;
	output.position = mul(vertex.position, mvpMatrix);
	float dist = length(mul(vertex.position, modelMatrix) - cameraPos);
	output.fog = clamp(fogCoord.x + dist * fogCoord.y, 0, 1.0f);
	output.normal = mul(vertex.normal, (float3x3)modelMatrix);
	output.color = vertex.color;
	output.color.a = 0;
	return output;
}
