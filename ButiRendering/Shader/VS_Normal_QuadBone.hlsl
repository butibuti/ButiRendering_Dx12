#include"DefaultShader.hlsli"

Use_ObjectMatrix(b0)
Use_Bone(b3)
Pixel_Normal VSMain(Vertex_Normal_QuadBone vertex)
{

	Pixel_Normal output;
	matrix bm = bones[vertex.boneIndex_1] + vertex.weight_02 * bones[vertex.boneIndex_2] + vertex.weight_03 * bones[vertex.boneIndex_3] + vertex.weight_04 * bones[vertex.boneIndex_4];

	//output.position = mul(bm, vertex.position);
	output.position = mul(vertex.position, mvpMatrix);

	output.normal = mul(vertex.normal, (float3x3)bm);
	output.normal = mul(output.normal, (float3x3)modelMatrix);

	//Pixel_Normal output;
	//output.position = mul(vertex.position, mvpMatrix);
	//output.normal = mul(vertex.normal, (float3x3)modelMatrix);
	return output;
}