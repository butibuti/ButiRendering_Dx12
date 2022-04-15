#include<vector>
#include"MeshPrimitive.h"
namespace ButiEngine {
	class MeshHelper
	{
	public:
		~MeshHelper();
		static void ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices);
		static Vector3 GetCircleTangent(std::int32_t i, std::int32_t tessellation);
		static Vector3 GetCircleVector(std::int32_t size, std::int32_t tessellation);
		
		//
		static void CreateTriangle(Vector3 point1, Vector3 point2, Vector3 point3, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateSameTextureCube(Vector3 size, const std::vector<Color>& arg_colors,bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateCube(Vector3 size, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData,bool flat);
		
		static void CreateSphere(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateSphereForParticle(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateCylinderCap(const std::vector<Color>& arg_colors,std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices, Vector3 size, std::int32_t tessellation, bool isTop);
		
		static void CreateCone(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateCapsule(Vector3 size, Vector3 pointA, Vector3 pointB, std::int32_t tessellation, bool isLie, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
				
		static void CreatePlane(const Vector2 size,const Vector3 offset,const float tilt,const float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		static void CreateReversiblePlane(const Vector2 size,const Vector3 offset,float tilt,  float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateHexergon(Vector2 size, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& outputVertecies);

		static void VertexAttachColor(const std::vector<Color>& arg_colors,std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_vertecies);

		static void CreateCirclePolygon(const float radius,const std::uint32_t	tessellation, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);

		static void CreateCameraFrustum(const float angle, const float width,const float height,const float nearclip, const float farclip, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);
		
		static void CreateImmediateMeshForParticle(const std::uint32_t arg_particleCount, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData);


		template<typename T, typename U>
		static void VertexConvert(const ButiRendering::MeshPrimitive<T>& inputMeshData, ButiRendering::MeshPrimitive<U>& outputMeshData) {
			outputMeshData.vertices.clear();
			outputMeshData.vertices.reserve(inputMeshData.vertices.size());
			outputMeshData.indices = inputMeshData.indices;
			for (std::int32_t i = 0,size= inputMeshData.vertices.size(); i <size ; i++) {
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
				outputMeshData.vertices.push_back(outVertex);
			}
			outputMeshData.eightCorner = inputMeshData.eightCorner;
		}

	private:
		MeshHelper();
	};
}

