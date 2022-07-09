#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_RendererStatus(b3)
Pixel_ShadowMapping VSMain(Vertex vertex)
{
	Pixel_ShadowMapping output;
	output.position = mul(vertex.position, mvpMatrix);
	output.depth.x = length(mul(vertex.position, modelMatrix) - cameraPos) * 0.01f;
	output.depth.y = output.depth.x * output.depth.x;
	return output;
}