#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_ObjectInformation(b1)
Use_RendererStatus(b3)
Pixel_Normal_Phong VSMain(Vertex_Normal vertex)
{
	Pixel_Normal_Phong output;
	output.position = mul(vertex.position, mvpMatrix);
	output.normal = normalize(mul(vertex.normal, (float3x3)modelMatrix));
	output.modelPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(vertex.position, modelMatrix);
	output.cameraPosition = mul(output.cameraPosition, viewMatrix);
	return output;
}