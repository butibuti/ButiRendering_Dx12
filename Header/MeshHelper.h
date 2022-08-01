#include<vector>
#include"MeshPrimitive.h"
namespace ButiEngine {
namespace ButiRendering {
class MeshHelper
{
public:
	~MeshHelper();
	BUTIRENDERING_API static void ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_vertices, std::vector<std::uint32_t>& arg_ref_indices);
	BUTIRENDERING_API static Vector3 GetCircleTangent(std::int32_t arg_index, std::int32_t arg_tessellation);
	BUTIRENDERING_API static Vector3 GetCircleVector(std::int32_t arg_size, std::int32_t arg_tessellation);

	//
	BUTIRENDERING_API static void CreateTriangle(const Vector3& arg_point1, const Vector3& arg_point2, const Vector3& arg_point3, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateSameTextureCube(Vector3 arg_size, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData,BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateCube(Vector3 arg_size, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner, const bool arg_flat);

	BUTIRENDERING_API static void CreateSphere(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateSphereForParticle(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateCylinderCap(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices, Vector3 arg_size, const std::int32_t arg_tessellation, const bool arg_isTop);

	BUTIRENDERING_API static void CreateCone(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateCapsule(Vector3 arg_size, const Vector3& arg_pointA, const Vector3& arg_pointB, const std::int32_t arg_tessellation, const bool isLie, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API  static void CreatePlane(const Vector2 arg_size, const const Vector3& offset, const float tilt, const float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);
	BUTIRENDERING_API  static void CreateReversiblePlane(const Vector2 arg_size, const const Vector3& offset, float tilt, float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateHexergon(Vector2 arg_size, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& outputVertecies);

	BUTIRENDERING_API static void VertexAttachColor(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_vertecies);

	BUTIRENDERING_API static void CreateCirclePolygon(const float arg_radius, const std::uint32_t	arg_tessellation, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateCameraFrustum(const float arg_angle, const float arg_width, const float arg_height, const float arg_nearClip, const float arg_farClip, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner);

	BUTIRENDERING_API static void CreateImmediateMeshForParticle(const std::uint32_t arg_particleCount, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData);


	template<typename T, typename U>
	static void VertexConvert(const ButiRendering::MeshPrimitive<T>& inputMeshData, ButiRendering::MeshPrimitive<U>& arg_ref_outputMeshData) {
		arg_ref_outputMeshData.vertices.clear();
		arg_ref_outputMeshData.vertices.reserve(inputMeshData.vertices.size());
		arg_ref_outputMeshData.SetIndex(inputMeshData.GetIndexValuePtr());
		for (std::int32_t i = 0, size = inputMeshData.vertices.size(); i < size; i++) {
			U outVertex;
			outVertex.position = inputMeshData.vertices.at(i).position;
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV, T> && std::is_base_of_v<Vertex::VertexInformation::UV, U>)
			{
				outVertex.uv = inputMeshData.vertices.at(i).uv;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::Normal, T> && std::is_base_of_v<Vertex::VertexInformation::Normal, U>)
			{
				outVertex.normal = inputMeshData.vertices.at(i).normal;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::Tangent, T> && std::is_base_of_v<Vertex::VertexInformation::Tangent, U>)
			{
				outVertex.tangent = inputMeshData.vertices.at(i).tangent;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::Color, T> && std::is_base_of_v<Vertex::VertexInformation::Color, U>)
			{
				outVertex.color = inputMeshData.vertices.at(i).color;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex1, T> && std::is_base_of_v<Vertex::VertexInformation::UV_ex1, U>)
			{
				outVertex.exuv1 = inputMeshData.vertices.at(i).exuv1;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex2, T> && std::is_base_of_v<Vertex::VertexInformation::UV_ex2, U>)
			{
				outVertex.exuv2 = inputMeshData.vertices.at(i).exuv2;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex3, T> && std::is_base_of_v<Vertex::VertexInformation::UV_ex2, U>)
			{
				outVertex.exuv3 = inputMeshData.vertices.at(i).exuv3;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::UV_ex4, T> && std::is_base_of_v<Vertex::VertexInformation::UV_ex3, U>)
			{
				outVertex.exuv4 = inputMeshData.vertices.at(i).exuv4;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::SingleBone, T> && std::is_base_of_v<Vertex::VertexInformation::SingleBone, U>)
			{
				outVertex.boneIndex = inputMeshData.vertices.at(i).boneIndex;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::DoubleBone, T> && std::is_base_of_v<Vertex::VertexInformation::DoubleBone, U>)
			{
				outVertex.boneIndex_1 = inputMeshData.vertices.at(i).boneIndex_1;
				outVertex.boneIndex_2 = inputMeshData.vertices.at(i).boneIndex_2;
				outVertex.weight = inputMeshData.vertices.at(i).weight;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::QuadBone, T> && std::is_base_of_v<Vertex::VertexInformation::QuadBone, U>)
			{
				outVertex.boneIndex_1 = inputMeshData.vertices.at(i).boneIndex_1;
				outVertex.boneIndex_2 = inputMeshData.vertices.at(i).boneIndex_2;
				outVertex.boneIndex_3 = inputMeshData.vertices.at(i).boneIndex_3;
				outVertex.boneIndex_4 = inputMeshData.vertices.at(i).boneIndex_4;
				outVertex.weight_1 = inputMeshData.vertices.at(i).weight_1;
				outVertex.weight_2 = inputMeshData.vertices.at(i).weight_2;
				outVertex.weight_3 = inputMeshData.vertices.at(i).weight_3;
				outVertex.weight_4 = inputMeshData.vertices.at(i).weight_4;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::SDEFBone, T> && std::is_base_of_v<Vertex::VertexInformation::SDEFBone, U>)
			{
				outVertex.boneIndex_1 = inputMeshData.vertices.at(i).boneIndex_1;
				outVertex.boneIndex_2 = inputMeshData.vertices.at(i).boneIndex_2;
				outVertex.weight = inputMeshData.vertices.at(i).weight;
				outVertex.SDEF_C = inputMeshData.vertices.at(i).SDEF_C;
				outVertex.SDEF_R0 = inputMeshData.vertices.at(i).SDEF_R0;
				outVertex.SDEF_R1 = inputMeshData.vertices.at(i).SDEF_R1;
			}
			if constexpr (std::is_base_of_v<Vertex::VertexInformation::PMX, T> && std::is_base_of_v<Vertex::VertexInformation::PMX, U>)
			{
				outVertex.boneIndex_1 = inputMeshData.vertices.at(i).boneIndex_1;
				outVertex.boneIndex_2 = inputMeshData.vertices.at(i).boneIndex_2;
				outVertex.boneIndex_3 = inputMeshData.vertices.at(i).boneIndex_3;
				outVertex.boneIndex_4 = inputMeshData.vertices.at(i).boneIndex_4;
				outVertex.weight_1 = inputMeshData.vertices.at(i).weight_1;
				outVertex.weight_2 = inputMeshData.vertices.at(i).weight_2;
				outVertex.weight_3 = inputMeshData.vertices.at(i).weight_3;
				outVertex.weight_4 = inputMeshData.vertices.at(i).weight_4;
				outVertex.SDEF_C = inputMeshData.vertices.at(i).SDEF_C;
				outVertex.SDEF_R0 = inputMeshData.vertices.at(i).SDEF_R0;
				outVertex.SDEF_R1 = inputMeshData.vertices.at(i).SDEF_R1;
			}
			arg_ref_outputMeshData.vertices.push_back(outVertex);
		}
	}

private:
	MeshHelper();
};
}
}

