#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Vertex_UV_Normal_Color VSMain(Vertex_UV_Normal_Color vertex)
{
	vertex.position = mul(vertex.position, modelMatrix);
	return vertex;
}