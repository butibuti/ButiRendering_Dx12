#include"stdafx.h"
#include "MeshHelper.h"
ButiEngine::MeshHelper::~MeshHelper()
{
}


void ButiEngine::MeshHelper::ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices)
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


ButiEngine::Vector3 ButiEngine::MeshHelper::GetCircleTangent(std::int32_t i, std::int32_t tessellation)
{
	float angle = (i * BM_2PI / tessellation) + BM_PIDIV2;
	float dx, dz;

	MathHelper::SinCos(dx, dz, angle);

	Vector3 v = Vector3( dx, 0, dz );
	return v;
}

ButiEngine::Vector3 ButiEngine::MeshHelper::GetCircleVector(std::int32_t size, std::int32_t tessellation)
{
	float angle = size * BM_2PI / tessellation;
	float dx, dz;

	MathHelper::SinCos(dx, dz, angle);

	Vector3 v = Vector3( dx, 0, dz );
	return v;

}



void ButiEngine::MeshHelper::CreateTriangle(Vector3 point1, Vector3 point2, Vector3 point3, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices{

	   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0.0f, 0.5f, 0.0f), Vector2(0.5f, 0.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)),
	   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)),
		   Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f),Vector3(0,0,-1.0f),Vector3(1.0f,0.0f,0.0f)) };
	std::vector<std::uint32_t>indices{
		0,1,2
	};
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateSameTextureCube(Vector3 size, const std::vector<Color>& arg_colors,bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	enum class ColorAttachType {
		SingleColor, AllColor, FourColor, SixColor
	};
	ColorAttachType colorType = ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	size.x *= 0.5f;
	size.y *= 0.5f;
	size.z *= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-size.x, size.y, -size.z) },
			{ Vector3(size.x,size.y, -size.z) },
			{ Vector3(-size.x, -size.y, -size.z) },
			{ Vector3(size.x, -size.y, -size.z) },
			{ Vector3(size.x,size.y, size.z) },
			{ Vector3(-size.x,size.y, size.z) },
			{ Vector3(size.x, -size.y, size.z) },
			{ Vector3(-size.x, -size.y, size.z) },
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
			if (flat) {
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

	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateCube(Vector3 size, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData, bool flat)
{
	enum class ColorAttachType {
	SingleColor,AllColor,FourColor,SixColor
	};
	ColorAttachType colorType=ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	size*= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-size.x, size.y, -size.z) },
			{ Vector3(size.x,size.y, -size.z) },
			{ Vector3(-size.x, -size.y, -size.z) },
			{ Vector3(size.x, -size.y, -size.z) },
			{ Vector3(size.x,size.y, size.z) },
			{ Vector3(-size.x,size.y, size.z) },
			{ Vector3(size.x, -size.y, size.z) },
			{ Vector3(-size.x, -size.y, size.z) },
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
			if (flat) {
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

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z);

	if (colorType == ColorAttachType::SingleColor)
		VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}


void ButiEngine::MeshHelper::CreateSphere(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData, true);
		return;
	}
	size = size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x,  size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3( size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back =  Vector3(-size.x,-size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x,-size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front =Vector3(size.x, -size.y, size.z);

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
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
			Vector3 pos = Vector3(dx * size.x, dy * size.y, dz * size.z);
			Vector3 tangent= normal != Vector3::YAxis?  normal.GetCross(Vector3::YAxis): Vector3::XAxis;
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



	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateSphereForParticle(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData, true);
		return;
	}
	size = size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z);

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * BM_PI / verticalSegments) - BM_PIDIV2;
		float dy, dxz;

		MathHelper::SinCos(dy, dxz, latitude);

		Vector3 CenterPos;

		if (i == 0||i== verticalSegments) {
			auto pos = Vector3(0, dy * size.y, 0);
			Vector3 normal = pos.GetNormalize();
			Vector3 tangent = normal != Vector3::YAxis ? normal.GetCross(Vector3::YAxis) : Vector3::XAxis;
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

			auto pos = Vector3(dx * size.x, dy * size.y, dz * size.z);
			Vector3 normal = pos.GetNormalize();
			Vector3 tangent = normal != Vector3::YAxis ? normal.GetCross(Vector3::YAxis) :Vector3::XAxis;
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pos, Vector2(u, v), normal,tangent));
		}
	}
	
	for (std::uint32_t i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}


	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateCylinderCap(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& ref_vertices, std::vector<std::uint32_t>& ref_indices, Vector3 size, std::int32_t tessellation, bool isTop)
{
	// indices.
	for (size_t i = 0; i < tessellation - 2; i++)
	{
		size_t i1 = (i + 1) % tessellation;
		size_t i2 = (i + 2) % tessellation;

		if (isTop)
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
	Vector3 tangent = normal != Vector3::YAxis ? normal.GetCross(Vector3::YAxis) :Vector3::XAxis;

	if (!isTop)
	{
		normal = -normal;
		textureScale.x *= -1;
	}
	// vertices.
	for (size_t i = 0; i < tessellation; i++)
	{
		Vector3 circleVector = GetCircleVector(i, tessellation);

		Vector3 position = (circleVector * size.x/2) + (normal * size.y);

		Vector2 textureCoordinate = Vector2(circleVector.x,circleVector.z) *(textureScale+Vector2(0.5,0.5)); 

		ref_vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(position, textureCoordinate, normal,tangent));
	}
}

void ButiEngine::MeshHelper::CreateCone(Vector3 size, std::int32_t tessellation, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	size.y /= 2;

	Vector3 topOffset = Vector3(0,size.y,0) ;

	float radius = size.x / 2;
	size_t stride = tessellation + 1;

	for (size_t i = 0; i <= tessellation; i++)
	{
		Vector3 circlevec = GetCircleVector(i, tessellation);

		Vector3 sideOffset = circlevec * radius;

		float u = (float)i / tessellation;

		Vector2 textureCoordinate =Vector2(u,u);
		textureCoordinate.y += 1.0f;
		Vector3 pt = sideOffset - topOffset;

		Vector3 normal = GetCircleTangent(i, tessellation).GetCross( topOffset - pt);
		normal.Normalize();
		Vector3 tangent = normal != Vector3::YAxis ?  normal.GetCross(Vector3::YAxis) :  Vector3::XAxis;
		outputMeshData.vertices .push_back(Vertex::Vertex_UV_Normal_Tangent_Color(topOffset, Vector2(0,0), normal,tangent));
		outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(pt, textureCoordinate, normal,tangent ));
		
		outputMeshData.indices.push_back((std::uint32_t)(i * 2));
		outputMeshData.indices.push_back((std::uint32_t)((i * 2 + 3) % (stride * 2)));
		outputMeshData.indices.push_back((std::uint32_t)((i * 2 + 1) % (stride * 2)));
	}

	CreateCylinderCap(arg_colors, outputMeshData.vertices, outputMeshData.indices, size, tessellation, false);
	//RHからLHに変更
	ReverseWinding(outputMeshData.vertices,outputMeshData.indices);

	VertexFlatConverter(outputMeshData.vertices);
	VertexAttachColor(arg_colors,outputMeshData.vertices);

}

void ButiEngine::MeshHelper::CreateCapsule(Vector3 size, Vector3 pointA, Vector3 pointB, std::int32_t tessellation, bool isLie, const std::vector<Color>& arg_colors, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData,true);
		return;
	}
	size = size * 0.5f;



	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z)+pointA;
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z) + pointA;
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z) + pointA;
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z) + pointA;

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z) + pointB;
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z) + pointB;
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z) + pointB;
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z) + pointB;

	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * BM_PI / verticalSegments) - BM_PIDIV2;
		float dy, dxz;

		MathHelper::SinCos(dy, dxz, latitude);

		Vector3 CenterPos = pointA;
		if (i >= (verticalSegments / 2)) {
			CenterPos = pointB;
		}

		float SphereTotalFront = size.x * BM_PI / 2.0f;
		float SylinderTotalFront = abs(pointA.y - pointB.y);
		float SphereRate = SphereTotalFront / (SphereTotalFront + SylinderTotalFront) / 2.0f;

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
			auto pos = Vector3(dx * size.x + CenterPos.x, dy * size.x + CenterPos.y, dz * size.x + CenterPos.z);
			Vector3 tangent = normal != Vector3::YAxis ?  normal.GetCross(Vector3::YAxis) :  Vector3::XAxis;
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



	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreatePlane(const Vector2 size,const Vector3 offset,const float tilt, const float UVMax, const std::uint32_t arg_verticalSeparate,  const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	ButiRendering::BoxSurface surface;
	surface.up = size.y * 0.5f+tilt;
	surface.down = -size.y * 0.5f;
	surface.right = size.x * 0.5f;
	surface.right = -size.x * 0.5f;
	surface.front = 0;
	surface.back = 0;


	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	float horizontalUnit = size.y / (arg_horizontalSeparate);
	float verticalUnit = size.x / (arg_verticalSeparate);




	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, 0)*0.5f;
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, 0) * 0.5f;

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, 0) * 0.5f;

	Vector3 normal = Vector3(0, 0, -1.0f);
	Vector3 tangent = normal != Vector3::YAxis ? tangent = normal.GetCross(Vector3::YAxis) : tangent = Vector3::XAxis;
	for (std::uint32_t i = 0; i < arg_horizontalSeparate + 1; i++) {
		for (std::int32_t j = 0; j < arg_verticalSeparate + 1; j++) {
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(verticalUnit * i - size.x / 2 +tilt*j , horizontalUnit * j - size.y / 2, 0)+offset, Vector2((verticalUnit * i / size.x), (1 - horizontalUnit * j / size.y)) * UVMax, normal,tangent));
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

	if (flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}



void ButiEngine::MeshHelper::CreateReversiblePlane(const Vector2 size, const Vector3 offset, float tilt, float UVMax, const std::uint32_t arg_verticalSeparate, const std::uint32_t arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	ButiRendering::BoxSurface surface;
	surface.up = size.y * 0.5f + tilt;
	surface.down = -size.y * 0.5f;
	surface.right = size.x * 0.5f;
	surface.right = -size.x * 0.5f;
	surface.front = 0;
	surface.back = 0;


	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	float horizontalUnit = size.y / (arg_horizontalSeparate);
	float verticalUnit = size.x / (arg_verticalSeparate);




	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, 0) * 0.5f;

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, 0) * 0.5f;

	Vector3 normal = Vector3(0,0,-1.0f);
	Vector3 tangent = normal != Vector3::YAxis ? tangent = normal.GetCross(Vector3::YAxis) : tangent = Vector3::XAxis;
	for (std::uint32_t i = 0; i < arg_horizontalSeparate + 1; i++) {
		for (std::int32_t j = 0; j < arg_verticalSeparate + 1; j++) {
			vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(verticalUnit * i - size.x / 2 + tilt * j, horizontalUnit * j - size.y / 2, 0) + offset, Vector2((verticalUnit * i / size.x), (1 - horizontalUnit * j / size.y)) * UVMax, normal,tangent));
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

	if (flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateHexergon(Vector2 size, const std::vector<Color>& arg_colors, bool flat, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	size.x *= 0.5f;
	size.y *= 0.5f;
	Vector3 normal = Vector3(0, 0, -1.0f);
	Vector3 tangent = normal != Vector3::YAxis ? tangent = normal.GetCross(Vector3::YAxis) : tangent = Vector3::XAxis;
	std::vector<Vertex::Vertex_UV_Normal_Tangent_Color> vertices
	{
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0, size.y, 0.0f),Vector2(0.0f,0.0f),normal,tangent),//0
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(size.x, size.y*0.5f, 0.0f),Vector2(1.0f,0.0f),normal,tangent),//1
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-size.x, size.y*0.5f, 0.0f),Vector2(0.0f,1.0f),normal,tangent),//2
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(size.x, -size.y*0.5f, 0.0f),Vector2(1.0f,1.0f),normal,tangent),//3
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(-size.x, -size.y*0.5f, 0.0f),Vector2(0.0f,1.0f),normal,tangent),//4
		Vertex::Vertex_UV_Normal_Tangent_Color(Vector3(0, -size.y, 0.0f),Vector2(1.0f,1.0f),normal,tangent),//5
	};
	std::vector<std::uint32_t>indices{
		0,1,2,
		1,3,2,
		2,3,4,
		5,4,3
	};
	if(flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}


void ButiEngine::MeshHelper::VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& outputVertices)
{
	for (auto itr = outputVertices.begin(); itr != outputVertices.end(); itr++) {
		itr->normal = itr->position.GetNormalize();
	}
}

void ButiEngine::MeshHelper::VertexAttachColor(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Tangent_Color>& arg_vertecies)
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

void ButiEngine::MeshHelper::CreateCirclePolygon(const float radius, const std::uint32_t tessellation, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	outputMeshData.vertices.clear();
	outputMeshData.indices.clear();
	if (tessellation < 3) {
		CreateCube(Vector3(radius, radius, radius), std::vector<Vector4>{}, outputMeshData, true);
		return;
	}
	std::vector< Vertex::Vertex_UV_Normal_Tangent_Color> vertices;
	std::vector<std::uint32_t> indices;
	std::vector<std::uint32_t> remainVertexIndex;
	std::vector<std::uint32_t> buffer;
	Vector3 normal = Vector3(0, 0, 1.0f);

	vertices.reserve(tessellation);
	indices.reserve((size_t)(tessellation - 2) * 3);
	remainVertexIndex.reserve(tessellation);
	buffer.reserve(tessellation);

	for (std::uint32_t i = 0; i < tessellation; i++) {
		float x = 0, y = 0;

		MathHelper::SinCos(x, y, BM_2PI / tessellation * i);
		auto  vertex= Vertex::Vertex_UV_Normal_Tangent_Color();

		vertex.position = Vector3(radius*x,radius*y,0);
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
	surface.up = radius;
	surface.down = -radius;
	surface.right = radius;
	surface.left = -radius;
	surface.front = 0;
	surface.back = 0;

	outputMeshData.vertices = vertices;
	outputMeshData.indices=indices ;
}

void ButiEngine::MeshHelper::CreateCameraFrustum(const float angle, const float width, const float height, const float nearclip, const float farclip, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	outputMeshData.Clear();
	Matrix4x4 projInv=Matrix4x4::PersepectiveFovLH(
		MathHelper::ToRadian(angle),
		width / height,
		nearclip,
		farclip
	);

	projInv.Inverse();


	Vector3 top;//0
	Vector3 leftup =( projInv*Vector4(-1, 1, 1,1)).GetVec3()*farclip;//1
	Vector3 leftbottom =( projInv * Vector4(-1, -1, 1, 1)).GetVec3() * farclip;//2
	Vector3 rightup = (projInv * Vector4(1, 1, 1, 1)).GetVec3() * farclip;//3
	Vector3 rightbottom =( projInv * Vector4(1, -1, 1, 1)).GetVec3() * farclip;//4

	Vector3 normal = Vector3(0, 0, -1.0f);
	outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(top, Vector2(), Vector3(0, 0, -1), Vector3::XAxis));
	outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(leftup, Vector2(), Vector3(0, 0, -1), Vector3::XAxis));
	outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(leftbottom, Vector2(), Vector3(0, 0, -1), Vector3::XAxis));
	outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(rightup, Vector2(), Vector3(0, 0, -1), Vector3::XAxis));
	outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Tangent_Color(rightbottom, Vector2(), Vector3(0, 0, -1), Vector3::XAxis));

	outputMeshData.indices.push_back(0);
	outputMeshData.indices.push_back(1);
	outputMeshData.indices.push_back(3);

	outputMeshData.indices.push_back(0);
	outputMeshData.indices.push_back(3);
	outputMeshData.indices.push_back(4);

	outputMeshData.indices.push_back(0);
	outputMeshData.indices.push_back(2);
	outputMeshData.indices.push_back(1);

	outputMeshData.indices.push_back(0);
	outputMeshData.indices.push_back(4);
	outputMeshData.indices.push_back(2);

	outputMeshData.indices.push_back(2);
	outputMeshData.indices.push_back(3);
	outputMeshData.indices.push_back(1);

	outputMeshData.indices.push_back(2);
	outputMeshData.indices.push_back(4);
	outputMeshData.indices.push_back(3);

	outputMeshData.eightCorner.up_left_front = Vector3(leftup);
	outputMeshData.eightCorner.up_right_front = Vector3(rightup);
	outputMeshData.eightCorner.down_left_front = Vector3(leftbottom);
	outputMeshData.eightCorner.down_right_front = Vector3(rightbottom);
	outputMeshData.eightCorner.up_left_back  =	Vector3();
	outputMeshData.eightCorner.up_right_back =	Vector3();
	outputMeshData.eightCorner.down_left_back =Vector3();
	outputMeshData.eightCorner.down_right_back=Vector3();
}

void ButiEngine::MeshHelper::CreateImmediateMeshForParticle(const std::uint32_t arg_particleCount, ButiRendering::MeshPrimitive<Vertex::Vertex_UV_Normal_Tangent_Color>& outputMeshData)
{
	outputMeshData.Clear();
	outputMeshData.vertices.reserve(arg_particleCount);
	outputMeshData.indices.reserve(arg_particleCount);
	auto vertex = Vertex::Vertex_UV_Normal_Tangent_Color();
	//vertex.position = Vector3(10000000000000000, 10000000000000000, 1000000000000000);
	//vertex.uv.x = 1.0;
	for (std::uint32_t i = 0; i < arg_particleCount; i++) {
		outputMeshData.vertices.push_back(vertex);
		outputMeshData.indices.push_back(i);
	}
}



