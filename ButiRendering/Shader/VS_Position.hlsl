#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Pixel VSMain(Vertex vertex)
{
	Pixel output;
	output.position = mul(vertex.position, mvpMatrix);
	return output;
}
