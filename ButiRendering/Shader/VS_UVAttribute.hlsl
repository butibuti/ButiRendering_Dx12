#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Vertex_UV VSMain(Vertex_UV vertex)
{
	vertex.position = mul(vertex.position, modelMatrix);
	return vertex;
}
