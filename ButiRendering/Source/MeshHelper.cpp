#include"stdafx.h"
#include "ButiRendering_Dx12/Header/MeshHelper.h"
ButiEngine::ButiRendering::MeshHelper::~MeshHelper()
{
}


void ButiEngine::ButiRendering::MeshHelper::ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices)
{
	if ((ref_indices.size() % 3) != 0)
		return;
	for (auto it = ref_indices.begin(); it != ref_indices.end(); it += 3)
	{
		std::swap(*it, *(it + 2));
	}
	for (auto it = ref_vertices.begin(); it != ref_vertices.end(); ++it)
	{
		it->uv.x = (1.f - it->uv.x);
	}
}


ButiEngine::Vector3 ButiEngine::ButiRendering::MeshHelper::GetCircleTangent(std::int32_t i, std::int32_t arg_tessellation)
{
	float angle = (i * BM_2PI / arg_tessellation) + BM_PIDIV2;
	float dx, dz;

	MathHelper::SinCos(dx, dz, angle);

	Vector3 v = Vector3( dx, 0, dz );
	return v;
}

ButiEngine::Vector3 ButiEngine::ButiRendering::MeshHelper::GetCircleVector(std::int32_t arg_size, std::int32_t arg_tessellation)
{
	float angle = arg_size * BM_2PI / arg_tessellation;
	float dx, dz;

	MathHelper::SinCos(dx, dz, angle);

	Vector3 v = Vector3( dx, 0, dz );
	return v;

}



void ButiEngine::ButiRendering::MeshHelper::CreateTriangle(const Vector3& arg_point1, const Vector3& arg_point2, const Vector3& arg_point3, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices{

	   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0.0f, 0.5f, 0.0f), Vector2(0.5f, 0.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)),
	   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)),
		   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)) };
	std::vector<std::uint32_t>indices{
		0,1,2
	};
	VertexAttachColor(arg_colors, vertices);
	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreateSameTextureCube(Vector3 arg_size, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	enum class ColorAttachType {
		SingleColor, AllColor, FourColor, SixColor
	};
	ColorAttachType colorType = ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	arg_size.x *= 0.5f;
	arg_size.y *= 0.5f;
	arg_size.z *= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-arg_size.x, arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x,arg_size.y, -arg_size.z) },
			{ Vector3(-arg_size.x, -arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x, -arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x,arg_size.y, arg_size.z) },
			{ Vector3(-arg_size.x,arg_size.y, arg_size.z) },
			{ Vector3(arg_size.x, -arg_size.y, arg_size.z) },
			{ Vector3(-arg_size.x, -arg_size.y, arg_size.z) },
	};
	std::vector<std::uint32_t> PosIndeces = {
		0, 1, 2, 3,
		1, 4, 3, 6,
		4, 5, 6, 7,
		5, 0, 7, 2,
		5, 4, 0, 1,
		2, 3, 7, 6,
	};
	std::vector<Vector2> vec_uv = {
		Vector2(0,0),
		Vector2(1,0),
		Vector2(0,1),
		Vector2(1,1),
	};

	std::vector<Vector3> FaceNormalVec = {
		{ Vector3(0, 0, -1.0f) },
		{ Vector3(1.0f, 0, 0) },
		{ Vector3(0, 0, 1.0f) },
		{ Vector3(-1.0f, 0, 0) },
		{ Vector3(0, 1.0f, 0) },
		{ Vector3(0, -1.0f, 0) }
	};

	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;
	std::uint32_t BasePosCount = 0;
	for (std::int32_t i = 0; i < 6; i++) {
		for (std::int32_t j = 0; j < 4; j++) {
			Vertex::Vertex_UV_Normal_Tangent_Color Data;
			Data.position = PosVec[PosIndeces[BasePosCount + j]];
			Data.uv = vec_uv.at(j);
			if (arg_flat) {
				//フラット表示の場合は法線は頂点方向にする
				Data.normal = Data.position;
				Data.normal.Normalize();
			}
			else {
				//フラット表示しない場合は、法線は面の向き
				Data.normal = FaceNormalVec[i];
			}
			if (colorType == ColorAttachType::SixColor) {
				Data.color = arg_colors.at(i);
			}
			else if (colorType == ColorAttachType::FourColor) {
				Data.color = arg_colors.at(j);

			}
			vertices.push_back(Data);
		}

		indices.push_back((std::uint32_t)BasePosCount + 0);
		indices.push_back((std::uint32_t)BasePosCount + 1);
		indices.push_back((std::uint32_t)BasePosCount + 2);
		indices.push_back((std::uint32_t)BasePosCount + 1);
		indices.push_back((std::uint32_t)BasePosCount + 3);
		indices.push_back((std::uint32_t)BasePosCount + 2);

		BasePosCount += 4;

	}
	if(colorType==ColorAttachType::SingleColor)
	VertexAttachColor(arg_colors, vertices);

	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, arg_size.z);
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, arg_size.z);

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, arg_size.z);
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, arg_size.z);
}

void ButiEngine::ButiRendering::MeshHelper::CreateCube(Vector3 arg_size, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner, const bool arg_flat)
{
	enum class ColorAttachType {
	SingleColor,AllColor,FourColor,SixColor
	};
	ColorAttachType colorType=ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	arg_size*= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-arg_size.x, arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x,arg_size.y, -arg_size.z) },
			{ Vector3(-arg_size.x, -arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x, -arg_size.y, -arg_size.z) },
			{ Vector3(arg_size.x,arg_size.y, arg_size.z) },
			{ Vector3(-arg_size.x,arg_size.y, arg_size.z) },
			{ Vector3(arg_size.x, -arg_size.y, arg_size.z) },
			{ Vector3(-arg_size.x, -arg_size.y, arg_size.z) },
	};
	std::vector<std::uint32_t> PosIndeces = {
		0, 1, 2, 3,
		1, 4, 3, 6,
		4, 5, 6, 7,
		5, 0, 7, 2,
		5, 4, 0, 1,
		2, 3, 7, 6,
	};
	std::vector<Vector2> vec_uv = {
		Vector2(0,0),
		Vector2(1,0),
		Vector2(0,1),
		Vector2(1,1),
	};

	std::vector<Vector3> FaceNormalVec = {
		{ Vector3(0, 0, -1.0f) },
		{ Vector3(1.0f, 0, 0) },
		{ Vector3(0, 0, 1.0f) },
		{ Vector3(-1.0f, 0, 0) },
		{ Vector3(0.01f, 1.0f, 0) },
		{ Vector3(0.01f, -1.0f, 0) }
	};
	std::vector<Vector4> TangentVec = {
		{ Vector4(1.0f, 0, 0,1.0f) },
		{ Vector4(0.0f, 0, -1.0,-1.0f) },
		{ Vector4(-1.0, 0, 0.0f,1.0f) },
		{ Vector4(0.0f, 0, 1.0,-1.0f) },
		{ Vector4(1.0f, 0.0f, 0,1.0f) },
		{ Vector4(-1.0f, 0.0f, 0,-1.0f) }
	};

	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;
	std::uint32_t BasePosCount = 0;
	for (std::int32_t i = 0; i < 6; i++) {
		for (std::int32_t j = 0; j < 4; j++) {
			Vertex::Vertex_UV_Normal_Tangent_Color Data;
			Data.position = PosVec[PosIndeces[BasePosCount + j]];
			Data.uv = vec_uv.at(j);
			if (arg_flat) {
				//フラット表示の場合は法線は頂点方向にする
				Data.normal = Data.position;
				Data.normal.Normalize();
			}
			else {
				//フラット表示しない場合は、法線は面の向き
				Data.normal = FaceNormalVec[i];
			}
			Data.color = TangentVec.at(i);
			vertices.push_back(Data);
		}

		indices.push_back((std::uint32_t)BasePosCount + 0);
		indices.push_back((std::uint32_t)BasePosCount + 1);
		indices.push_back((std::uint32_t)BasePosCount + 2);
		indices.push_back((std::uint32_t)BasePosCount + 1);
		indices.push_back((std::uint32_t)BasePosCount + 3);
		indices.push_back((std::uint32_t)BasePosCount + 2);

		BasePosCount += 4;

	}

	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, arg_size.z);
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, arg_size.z);

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, arg_size.z);
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, arg_size.z);

	if (colorType == ColorAttachType::SingleColor)
		VertexAttachColor(arg_colors, vertices);
	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}


void ButiEngine::ButiRendering::MeshHelper::CreateSphere(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	if (arg_tessellation < 3) {
		CreateCube(arg_size, arg_colors, arg_ref_outputMeshData,arg_ref_eightCorner, true);
		return;
	}
	arg_size = arg_size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x,  arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, arg_size.z);
	arg_ref_eightCorner.up_right_back = Vector3( arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, arg_size.z);

	arg_ref_eightCorner.down_left_back =  Vector3(-arg_size.x,-arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x,-arg_size.y, arg_size.z);
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_right_front =Vector3(arg_size.x, -arg_size.y, arg_size.z);

	size_t verticalSegments = arg_tessellation;
	size_t horizontalSegments = arg_tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * BM_PI / verticalSegments) - BM_PIDIV2;
		float dy, dxz;

		MathHelper::SinCos(dy, dxz, latitude);

		Vector3 CenterPos;



		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * BM_2PI / horizontalSegments;
			float dx, dz;

			MathHelper::SinCos(dx, dz, longitude);

			dx *= dxz;
			dz *= dxz;

			Vector3 normal = Vector3(dx, dy, dz).GetNormalize();
			Vector3 pos = Vector3(dx * arg_size.x, dy * arg_size.y, dz * arg_size.z);
			Vector3 tangent= normal != Vector3Const::YAxis?  normal.GetCross(Vector3Const::YAxis): Vector3Const::XAxis;
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pos, Vector2(u, v), normal,tangent));
		}
	}
	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indices.push_back((std::uint32_t)(i * stride + j));
			indices.push_back((std::uint32_t)(nextI * stride + j));
			indices.push_back((std::uint32_t)(i * stride + nextJ));

			indices.push_back((std::uint32_t)(i * stride + nextJ));
			indices.push_back((std::uint32_t)(nextI * stride + j));
			indices.push_back((std::uint32_t)(nextI * stride + nextJ));
		}
	}
	ReverseWinding(vertices, indices);



	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreateSphereForParticle(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	if (arg_tessellation < 3) {
		CreateCube(arg_size, arg_colors, arg_ref_outputMeshData,arg_ref_eightCorner, true);
		return;
	}
	arg_size = arg_size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, arg_size.z);
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, -arg_size.z);
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, arg_size.z);

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, arg_size.z);
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, -arg_size.z);
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, arg_size.z);

	size_t verticalSegments = arg_tessellation;
	size_t horizontalSegments = arg_tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * BM_PI / verticalSegments) - BM_PIDIV2;
		float dy, dxz;

		MathHelper::SinCos(dy, dxz, latitude);

		Vector3 CenterPos;

		if (i == 0||i== verticalSegments) {
			auto pos = Vector3(0, dy * arg_size.y, 0);
			Vector3 normal = pos.GetNormalize();
			Vector3 tangent = normal != Vector3Const::YAxis ? normal.GetCross(Vector3Const::YAxis) : Vector3Const::XAxis;
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pos, Vector2(0, v), normal,tangent));
			continue;
		}

		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * BM_2PI / horizontalSegments;
			float dx, dz;

			MathHelper::SinCos(dx, dz, longitude);

			dx *= dxz;
			dz *= dxz;

			auto pos = Vector3(dx * arg_size.x, dy * arg_size.y, dz * arg_size.z);
			Vector3 normal = pos.GetNormalize();
			Vector3 tangent = normal != Vector3Const::YAxis ? normal.GetCross(Vector3Const::YAxis) :Vector3Const::XAxis;
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pos, Vector2(u, v), normal,tangent));
		}
	}
	
	for (std::uint32_t i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}


	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreateCylinderCap(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices, Vector3 arg_size, const std::int32_t arg_tessellation, const bool arg_isTop)
{
	// indices.
	for (size_t i = 0; i < arg_tessellation - 2; i++)
	{
		size_t i1 = (i + 1) % arg_tessellation;
		size_t i2 = (i + 2) % arg_tessellation;

		if (arg_isTop)
		{
			std::swap(i1, i2);
		}

		size_t vbase = ref_vertices.size();
		ref_indices.push_back((std::uint32_t)vbase);
		ref_indices.push_back((std::uint32_t)(vbase + i1));
		ref_indices.push_back((std::uint32_t)(vbase + i2));
	}

	Vector3 normal = Vector3(0,1,0);
	Vector2 textureScale =Vector2 (-0.5f,-0.5f);
	Vector3 tangent = normal != Vector3Const::YAxis ? normal.GetCross(Vector3Const::YAxis) :Vector3Const::XAxis;

	if (!arg_isTop)
	{
		normal = -normal;
		textureScale.x *= -1;
	}
	// vertices.
	for (size_t i = 0; i < arg_tessellation; i++)
	{
		Vector3 circleVector = GetCircleVector(i, arg_tessellation);

		Vector3 position = (circleVector * arg_size.x/2) + (normal * arg_size.y);

		Vector2 textureCoordinate = Vector2(circleVector.x,circleVector.z) *(textureScale+Vector2(0.5,0.5)); 

		ref_vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(position, textureCoordinate, normal,tangent));
	}
}

void ButiEngine::ButiRendering::MeshHelper::CreateCone(Vector3 arg_size, const std::int32_t arg_tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	arg_size.y /= 2;
	std::vector<std::uint32_t> vec_index;
	Vector3 topOffset = Vector3(0,arg_size.y,0) ;

	float arg_radius = arg_size.x / 2;
	size_t stride = arg_tessellation + 1;

	for (size_t i = 0; i <= arg_tessellation; i++)
	{
		Vector3 circlevec = GetCircleVector(i, arg_tessellation);

		Vector3 sideOffset = circlevec * arg_radius;

		float u = (float)i / arg_tessellation;

		Vector2 textureCoordinate =Vector2(u,u);
		textureCoordinate.y += 1.0f;
		Vector3 pt = sideOffset - topOffset;

		Vector3 normal = GetCircleTangent(i, arg_tessellation).GetCross( topOffset - pt);
		normal.Normalize();
		Vector3 tangent = normal != Vector3Const::YAxis ?  normal.GetCross(Vector3Const::YAxis) :  Vector3Const::XAxis;
		arg_ref_outputMeshData.vertices .push_back(Vertex::Vertex_UV_Normal_Tangent_Color(topOffset, Vector2(0,0), normal,tangent));
		arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pt, textureCoordinate, normal,tangent ));
		
		vec_index.push_back((std::uint32_t)(i * 2));
		vec_index.push_back((std::uint32_t)((i * 2 + 3) % (stride * 2)));
		vec_index.push_back((std::uint32_t)((i * 2 + 1) % (stride * 2)));
	}

	CreateCylinderCap(arg_colors, arg_ref_outputMeshData.vertices, vec_index, arg_size, arg_tessellation, false);
	//RHからLHに変更
	ReverseWinding(arg_ref_outputMeshData.vertices, vec_index);

	VertexFlatConverter(arg_ref_outputMeshData.vertices);
	VertexAttachColor(arg_colors,arg_ref_outputMeshData.vertices);
	arg_ref_outputMeshData.SetIndex(vec_index);
}

void ButiEngine::ButiRendering::MeshHelper::CreateCapsule(Vector3 arg_size, const Vector3& arg_pointA, const Vector3& arg_pointB, const std::int32_t arg_tessellation, const bool isLie, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	if (arg_tessellation < 3) {
		CreateCube(arg_size, arg_colors, arg_ref_outputMeshData,arg_ref_eightCorner,true);
		return;
	}
	arg_size = arg_size * 0.5f;



	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, -arg_size.z)+arg_pointA;
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, arg_size.z) + arg_pointA;
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, -arg_size.z) + arg_pointA;
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, arg_size.z) + arg_pointA;

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, -arg_size.z) + arg_pointB;
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, arg_size.z) + arg_pointB;
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, -arg_size.z) + arg_pointB;
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, arg_size.z) + arg_pointB;

	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	size_t verticalSegments = arg_tessellation;
	size_t horizontalSegments = arg_tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * BM_PI / verticalSegments) - BM_PIDIV2;
		float dy, dxz;

		MathHelper::SinCos(dy, dxz, latitude);

		Vector3 CenterPos = arg_pointA;
		if (i >= (verticalSegments / 2)) {
			CenterPos = arg_pointB;
		}

		float SphereTotalFront = arg_size.x * BM_PI*0.5f;
		float SylinderTotalFront = abs(arg_pointA.y - arg_pointB.y);
		float SphereRate = SphereTotalFront / (SphereTotalFront + SylinderTotalFront) *0.5f;

		if (v <= 0.5f) {
			v *= SphereRate;
		}
		else {
			v = 1.0f - ((1.0f - v) * SphereRate);
		}


		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * BM_2PI / horizontalSegments;
			float dx, dz;

			MathHelper::SinCos(dx, dz, longitude);

			dx *= dxz;
			dz *= dxz;

			auto normal = Vector3(dx, dy, dz);
			auto pos = Vector3(dx * arg_size.x + CenterPos.x, dy * arg_size.x + CenterPos.y, dz * arg_size.x + CenterPos.z);
			Vector3 tangent = normal != Vector3Const::YAxis ?  normal.GetCross(Vector3Const::YAxis) :  Vector3Const::XAxis;
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pos, Vector2(u, v),normal,tangent));
		}
	}
	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indices.push_back((std::uint32_t)(i * stride + j));
			indices.push_back((std::uint32_t)(nextI * stride + j));
			indices.push_back((std::uint32_t)(i * stride + nextJ));

			indices.push_back((std::uint32_t)(i * stride + nextJ));
			indices.push_back((std::uint32_t)(nextI * stride + j));
			indices.push_back((std::uint32_t)(nextI * stride + nextJ));
		}
	}
	ReverseWinding(vertices, indices);

	if (isLie) {
		for (auto& v : vertices) {
			float tmp = -v.position.y;
			v.position.y = v.position.x;
			v.position.x = tmp;
			float uvTmp = -v.uv.y;
			v.uv.y = v.uv.x;
			v.uv.x = uvTmp;
		}
	}



	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreatePlane(const Vector2 arg_size, const const Vector3& offset, const float tilt, const float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	ButiRendering::BoxSurface surface;
	surface.up = arg_size.y * 0.5f+tilt;
	surface.down = -arg_size.y * 0.5f;
	surface.right = arg_size.x * 0.5f;
	surface.right = -arg_size.x * 0.5f;
	surface.front = 0;
	surface.back = 0;


	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	float horizontalUnit = arg_size.y / (arg_horizontalSeparate);
	float verticalUnit = arg_size.x / (arg_verticalSeparate);




	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, 0)*0.5f;
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, 0) * 0.5f;

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, 0) * 0.5f;

	Vector3 normal = Vector3(0, 0, -1.0f);
	Vector3 tangent = normal != Vector3Const::YAxis ? tangent = normal.GetCross(Vector3Const::YAxis) : tangent = Vector3Const::XAxis;
	for (std::uint32_t i = 0; i < arg_horizontalSeparate + 1; i++) {
		for (std::int32_t j = 0; j < arg_verticalSeparate + 1; j++) {
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(verticalUnit * i - arg_size.x / 2 + tilt * j, horizontalUnit * j - arg_size.y / 2, 0) + offset, Vector2((verticalUnit * i / arg_size.x), (1 - horizontalUnit * j / arg_size.y)) * UVMax, normal, tangent));
		}
	}



	std::vector<std::uint32_t>indices;
	for (std::uint32_t i = 0; i < (arg_horizontalSeparate + 1) * arg_verticalSeparate; i += arg_horizontalSeparate + 1) {
		for (std::uint32_t j = i; j < i + arg_horizontalSeparate; j++) {
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j + arg_horizontalSeparate + 1);
			indices.push_back(j);
			indices.push_back(j + 1);
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j);
		}
	}

	if (arg_flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}



void ButiEngine::ButiRendering::MeshHelper::CreateReversiblePlane(const Vector2 arg_size, const const Vector3& offset, float tilt, float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	ButiRendering::BoxSurface surface;
	surface.up = arg_size.y * 0.5f + tilt;
	surface.down = -arg_size.y * 0.5f;
	surface.right = arg_size.x * 0.5f;
	surface.right = -arg_size.x * 0.5f;
	surface.front = 0;
	surface.back = 0;


	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	float horizontalUnit = arg_size.y / (arg_horizontalSeparate);
	float verticalUnit = arg_size.x / (arg_verticalSeparate);




	arg_ref_eightCorner.up_left_back = Vector3(-arg_size.x, arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.up_left_front = Vector3(-arg_size.x, arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.up_right_back = Vector3(arg_size.x, arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.up_right_front = Vector3(arg_size.x, arg_size.y, 0) * 0.5f;

	arg_ref_eightCorner.down_left_back = Vector3(-arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_left_front = Vector3(-arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_right_back = Vector3(arg_size.x, -arg_size.y, 0) * 0.5f;
	arg_ref_eightCorner.down_right_front = Vector3(arg_size.x, -arg_size.y, 0) * 0.5f;

	Vector3 normal = Vector3(0,0,-1.0f);
	Vector3 tangent = normal != Vector3Const::YAxis ? tangent = normal.GetCross(Vector3Const::YAxis) : tangent = Vector3Const::XAxis;
	for (std::uint32_t i = 0; i < arg_horizontalSeparate + 1; i++) {
		for (std::int32_t j = 0; j < arg_verticalSeparate + 1; j++) {
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(verticalUnit * i - arg_size.x / 2 + tilt * j, horizontalUnit * j - arg_size.y / 2, 0) + offset, Vector2((verticalUnit * i / arg_size.x), (1 - horizontalUnit * j / arg_size.y)) * UVMax, normal,tangent));
		}
	}



	std::vector<std::uint32_t>indices;
	for (std::uint32_t i = 0; i < (arg_horizontalSeparate + 1) * arg_verticalSeparate; i += arg_horizontalSeparate + 1) {
		for (std::uint32_t j = i; j < i + arg_horizontalSeparate; j++) {
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j + arg_horizontalSeparate + 1);
			indices.push_back(j);
			indices.push_back(j + 1);
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j);

			indices.push_back(j);
			indices.push_back(j + arg_horizontalSeparate + 1);
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j);
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j + 1);
		}
	}

	if (arg_flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreateHexergon(Vector2 arg_size, const std::vector<Color>& arg_colors, const bool arg_flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	arg_size.x *= 0.5f;
	arg_size.y *= 0.5f;
	Vector3 normal = Vector3(0, 0, -1.0f);
	Vector3 tangent = normal != Vector3Const::YAxis ? tangent = normal.GetCross(Vector3Const::YAxis) : tangent = Vector3Const::XAxis;
	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices
	{
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0, arg_size.y, 0.0f),Vector2(0.0f,0.0f),normal,tangent),//0
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(arg_size.x, arg_size.y*0.5f, 0.0f),Vector2(1.0f,0.0f),normal,tangent),//1
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-arg_size.x, arg_size.y*0.5f, 0.0f),Vector2(0.0f,1.0f),normal,tangent),//2
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(arg_size.x, -arg_size.y*0.5f, 0.0f),Vector2(1.0f,1.0f),normal,tangent),//3
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-arg_size.x, -arg_size.y*0.5f, 0.0f),Vector2(0.0f,1.0f),normal,tangent),//4
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0, -arg_size.y, 0.0f),Vector2(1.0f,1.0f),normal,tangent),//5
	};
	std::vector<std::uint32_t>indices{
		0,1,2,
		1,3,2,
		2,3,4,
		5,4,3
	};
	if(arg_flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}


void ButiEngine::ButiRendering::MeshHelper::VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& outputVertices)
{
	for (auto itr = outputVertices.begin(); itr != outputVertices.end(); itr++) {
		itr->normal = itr->position.GetNormalize();
	}
}

void ButiEngine::ButiRendering::MeshHelper::VertexAttachColor(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_vertecies)
{
	if (arg_colors.size() <= 0) {
		return;
	}
	if (arg_colors.size() == arg_vertecies.size()) {
		for (std::int32_t i = 0; i < arg_vertecies.size(); i++) {
			arg_vertecies.at(i).color = arg_colors.at(i);
		}
		return;
	}
	for (std::int32_t i = 0; i < arg_vertecies.size(); i++) {
		arg_vertecies.at(i).color = arg_colors.at(0);
	}
	return;
}

void ButiEngine::ButiRendering::MeshHelper::CreateCirclePolygon(const float arg_radius, const std::uint32_t	arg_tessellation, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	arg_ref_outputMeshData.Clear();
	if (arg_tessellation < 3) {
		CreateCube(Vector3(arg_radius, arg_radius, arg_radius), std::vector<Vector4>{}, arg_ref_outputMeshData,arg_ref_eightCorner, true);
		return;
	}
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<std::uint32_t> remainVertexIndex;
	std::vector<std::uint32_t> buffer;
	Vector3 normal = Vector3(0, 0, 1.0f);

	vertices.reserve(arg_tessellation);
	indices.reserve((size_t)(arg_tessellation - 2) * 3);
	remainVertexIndex.reserve(arg_tessellation);
	buffer.reserve(arg_tessellation);

	for (std::uint32_t i = 0; i < arg_tessellation; i++) {
		float x = 0, y = 0;

		MathHelper::SinCos(x, y, BM_2PI / arg_tessellation * i);
		auto  vertex= Vertex::Vertex_UV_Normal_Tangent_Color();

		vertex.position = Vector3(arg_radius*x,arg_radius*y,0);
		vertex.uv = Vector2((1+x)/2,1- (1+y)/2);
		vertex.normal = normal;


		vertices.push_back(vertex);
		
		remainVertexIndex.push_back(i);
	}
	std::int32_t offset = 0;
	do
	{
		offset = 0;
		for (; offset < remainVertexIndex.size();) {
			buffer.push_back(remainVertexIndex.at(offset));
			if ((offset + 2) > (remainVertexIndex.size() )) {
				break;
			}

			indices.push_back(remainVertexIndex.at(offset));
			indices.push_back(remainVertexIndex.at(offset+1));
			if((offset + 2)>remainVertexIndex.size()-1)
				indices.push_back(remainVertexIndex.at(0));
			else {
				indices.push_back(remainVertexIndex.at(offset + 2));
			}

			offset += 2;
		}
		remainVertexIndex = buffer;
		buffer.clear();
		buffer.reserve(remainVertexIndex.size());
	} while (remainVertexIndex.size()>=3);

	ButiRendering::BoxSurface surface;
	surface.up = arg_radius;
	surface.down = -arg_radius;
	surface.right = arg_radius;
	surface.left = -arg_radius;
	surface.front = 0;
	surface.back = 0;

	arg_ref_outputMeshData.vertices = vertices;
	arg_ref_outputMeshData.SetIndex(indices);
}

void ButiEngine::ButiRendering::MeshHelper::CreateCameraFrustum(const float arg_angle, const float arg_width, const float arg_height, const float arg_nearClip, const float arg_farClip, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData, BoxEightCorner& arg_ref_eightCorner)
{
	arg_ref_outputMeshData.Clear();
	std::vector<std::uint32_t> vec_index;
	Matrix4x4 projInv=Matrix4x4::PersepectiveFovLH(
		MathHelper::ToRadian(arg_angle),
		arg_width / arg_height,
		arg_nearClip,
		arg_farClip
	);

	projInv.Inverse();


	Vector3 top;//0
	Vector3 leftup =( projInv*Vector4(-1, 1, 1,1)).GetVec3()*arg_farClip;//1
	Vector3 leftbottom =( projInv * Vector4(-1, -1, 1, 1)).GetVec3() * arg_farClip;//2
	Vector3 rightup = (projInv * Vector4(1, 1, 1, 1)).GetVec3() * arg_farClip;//3
	Vector3 rightbottom =( projInv * Vector4(1, -1, 1, 1)).GetVec3() * arg_farClip;//4

	Vector3 normal = Vector3(0, 0, -1.0f);
	arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(top, Vector2(), Vector3(0, 0, -1), Vector3Const::XAxis));
	arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(leftup, Vector2(), Vector3(0, 0, -1), Vector3Const::XAxis));
	arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(leftbottom, Vector2(), Vector3(0, 0, -1), Vector3Const::XAxis));
	arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(rightup, Vector2(), Vector3(0, 0, -1), Vector3Const::XAxis));
	arg_ref_outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(rightbottom, Vector2(), Vector3(0, 0, -1), Vector3Const::XAxis));

	vec_index.push_back(0);
	vec_index.push_back(1);
	vec_index.push_back(3);

	vec_index.push_back(0);
	vec_index.push_back(3);
	vec_index.push_back(4);

	vec_index.push_back(0);
	vec_index.push_back(2);
	vec_index.push_back(1);

	vec_index.push_back(0);
	vec_index.push_back(4);
	vec_index.push_back(2);

	vec_index.push_back(2);
	vec_index.push_back(3);
	vec_index.push_back(1);

	vec_index.push_back(2);
	vec_index.push_back(4);
	vec_index.push_back(3);
	arg_ref_outputMeshData.SetIndex(vec_index);
	arg_ref_eightCorner.up_left_front = Vector3(leftup);
	arg_ref_eightCorner.up_right_front = Vector3(rightup);
	arg_ref_eightCorner.down_left_front = Vector3(leftbottom);
	arg_ref_eightCorner.down_right_front = Vector3(rightbottom);
	arg_ref_eightCorner.up_left_back  =	Vector3();
	arg_ref_eightCorner.up_right_back =	Vector3();
	arg_ref_eightCorner.down_left_back =Vector3();
	arg_ref_eightCorner.down_right_back=Vector3();
}

void ButiEngine::ButiRendering::MeshHelper::CreateImmediateMeshForParticle(const std::uint32_t arg_particleCount, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_ref_outputMeshData)
{
	arg_ref_outputMeshData.Clear();
	arg_ref_outputMeshData.vertices.reserve(arg_particleCount);
	std::vector<std::uint32_t> vec_index;
	vec_index.reserve(arg_particleCount);
	auto vertex = Vertex::Vertex_UV_Normal_Tangent_Color();
	//vertex.position = Vector3(10000000000000000, 10000000000000000, 1000000000000000);
	//vertex.uv.x = 1.0;
	for (std::uint32_t i = 0; i < arg_particleCount; i++) {
		arg_ref_outputMeshData.vertices.push_back(vertex);
		vec_index.push_back(i);
	}
	arg_ref_outputMeshData.SetIndex(vec_index);
}



