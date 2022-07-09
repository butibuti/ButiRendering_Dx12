#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Pixel_UV VSMain(Vertex_UV vertex)
{
	Pixel_UV output;
	output.position = mul(vertex.position, mvpMatrix);
	output.uv = vertex.uv;
	return output;
}
