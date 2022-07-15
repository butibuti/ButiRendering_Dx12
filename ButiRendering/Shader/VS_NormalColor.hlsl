#include"DefaultShader.hlsli"
Use_ObjectMatrix(b0)
Pixel_Normal_Color VSMain(Vertex_Normal_Color vertex)
{
	Pixel_Normal_Color output;
	output.position = mul(vertex.position, modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = mul(vertex.normal, (float3x3)modelMatrix);
	output.color = vertex.color;
	output.color.a = 0;
	return output;
}
