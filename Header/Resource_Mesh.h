#pragma once
#include"ButiRendering_Dx12/Header/Vertex.h"
#include"ResourceInterface.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice;
class Resource_Mesh : public GPUResource, public IResource_Mesh
{
public:
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	void Initialize()override {}
	void PreInitialize()override {}
	BUTIRENDERING_API void SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)override;
	BUTIRENDERING_API std::uint32_t GetVertexCount();
	BUTIRENDERING_API std::uint32_t GetIndexCount();
	BUTIRENDERING_API bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line);
	BUTIRENDERING_API const ButiRendering::MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const override;
	BUTIRENDERING_API const std::string& GetTagName()const override;
	BUTIRENDERING_API void SetTagName(const std::string& arg_tagName);

	const BoxEightCorner& GetBoxEightCorner()const override{
		return eightCorner;
	}
	void SetBoxSurfaces(const BoxSurface& arg_boxSurface)override {
		eightCorner.up_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.front);
		eightCorner.up_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.front);
		eightCorner.up_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.back);
		eightCorner.up_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.back);
		eightCorner.down_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.front);
		eightCorner.down_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.front);
		eightCorner.down_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.back);
		eightCorner.down_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.back);
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
	std::string tagName;
	std::map<std::uint32_t, Value_ptr<ButiRendering::MeshPrimitiveBase>> map_vlp_meshPrimitive;

	BoxEightCorner eightCorner = BoxEightCorner();
};
}
}