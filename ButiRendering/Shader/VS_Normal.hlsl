#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)

Pixel_Normal VSMain(Vertex_Normal vertex)
{
	Pixel_Normal output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal =  mul(vertex.normal, (float3x3)modelMatrix);

	return output;
}