#pragma once
#include"stdafx.h"
#include"ResourceInterface.h"
#include"ButiRendering_Dx12/Header/vertex.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice;
class Resource_DynamicMesh : public GPUResource, public IResource_Mesh
{
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	void Initialize()override {}
	void PreInitialize()override {}
	BUTIRENDERING_API void SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)override;
	BUTIRENDERING_API std::uint32_t GetVertexCount()override;
	BUTIRENDERING_API std::uint32_t GetIndexCount()override;
	BUTIRENDERING_API virtual void Update() = 0;
	BUTIRENDERING_API const ButiRendering::MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const override;
	BUTIRENDERING_API Value_ptr<ButiRendering::MeshPrimitiveBase> GetBackupData_Row();

	BUTIRENDERING_API bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line);
	BUTIRENDERING_API const std::string& GetTagName()const override;
	BUTIRENDERING_API void SetTagName(const std::string& arg_tagName);

	const BoxEightCorner& GetBoxEightCorner()const override {
		return eightCorner;
	}
	void SetBoxSurfaces(const BoxSurface& arg_boxSurface)override {
		eightCorner = BoxEightCorner(arg_boxSurface);
	}
	void SetBoxEightCorner(const BoxEightCorner& arg_boxcorner)override {
		eightCorner = arg_boxcorner;
	}
protected:
	std::uint32_t verticesCount;
	std::uint32_t indexCount;
	std::uint32_t stride;
	bool canAccessWrite;
	bool isDataRefresh = true;
	std::uint32_t vertexType;
	Value_ptr<ButiRendering::MeshPrimitiveBase> vlp_meshPrimitive;
	std::string tagName;
	BoxEightCorner eightCorner;
};
}
}