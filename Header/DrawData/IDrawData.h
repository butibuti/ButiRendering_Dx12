#ifndef IDrawData_H
#define IDrawData_H
#pragma once
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
#include"../CBuffer.h"
#include"IDrawObject.h"
namespace ButiEngine {

namespace ButiRendering {
struct DrawInformation :public IObject {
	DrawInformation() {}
	std::uint32_t layer = 0;
	void Initialize()override {}
	void PreInitialize()override {}
	std::vector<Value_ptr<ICBuffer>> vec_exCBuffer;
	DrawFixParam drawFixParam = DrawFixParam::none;
	BillBoardMode billboardMode = BillBoardMode::none;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(layer);
		archive(vec_exCBuffer);
		archive(billboardMode);
		archive(drawFixParam);
	}
	bool IsContainExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
			if ((*itr)->GetExName() == arg_cbufferName) {
				return true;
			}
		}
		return false;
	}
	void RemoveExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
			if ((*itr)->GetExName() == arg_cbufferName) {
				vec_exCBuffer.erase(itr);
				return;
			}
			else
				itr++;
		}
	}
	Value_ptr<ICBuffer> GetExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
			if ((*itr)->GetExName() == arg_cbufferName) {
				return *itr;
			}
			else {
				itr++;
			}
		}
		return nullptr;
	}

	Value_ptr< DrawInformation > Clone() {
		auto output = ObjectFactory::Create<DrawInformation>();
		output->layer = layer;
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
			output->vec_exCBuffer.push_back((*itr)->Clone());
		}

		return output;
	}
};
class DrawData {
public:
	BUTIRENDERING_API void SetBlendMode(const BlendMode& arg_BlendMode);
	BUTIRENDERING_API void MatrixUpdate();
	Matrix4x4 transform;
	Value_ptr<Transform> vlp_transform;

	inline float GetMaxZ(const Matrix4x4& arg_viewMatrix) {
		auto viewPos = transform.GetPosition_Transpose() * arg_viewMatrix;
		return viewPos.z;
	}
	std::vector<std::uint32_t> subset;
	Value_ptr< DrawInformation >vlp_drawInfo;
	Value_ptr<IRenderer> vlp_renderer;
	inline Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
		vlp_drawInfo->vec_exCBuffer.push_back(arg_cbuffer);
		return arg_cbuffer;
	}
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {

		for (auto itr = vlp_drawInfo->vec_exCBuffer.begin(); itr != vlp_drawInfo->vec_exCBuffer.end(); itr++) {
			if ((*itr)->GetExName() == arg_bufferName) {
				return *itr;
			}
		}

		return nullptr;
	}

	template <class T>
	inline Value_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_bufferName) {

		auto out = GetICBuffer(arg_bufferName);
		if (out && out->IsThis<CBuffer<T>>()) {
			return out->GetThis<CBuffer<T>>();
		}

		return nullptr;
	}
	Value_ptr < CBuffer<Matrices>>& GetCBuffer() { return cbuffer; }
	void SetCbuffer(const Value_ptr < CBuffer<Matrices>>& arg_cBuffer) { cbuffer = arg_cBuffer; }
	std::uint32_t* GetCollisionIndex() { return p_collisionRegistIndex; }
	void SetCollisionIndex(std::uint32_t* arg_p_collisionIndex) { p_collisionRegistIndex = arg_p_collisionIndex; }
	std::uint32_t GetCBufferCount()const { return cBufferCount; }
	void SetCBufferCount(const std::uint32_t arg_count) { cBufferCount = arg_count; }

	Value_weak_ptr<IResource_Model> GetModel() const { return vwp_model; }
	Value_weak_ptr<IResource_Mesh> GetMesh()const { return vwp_mesh; }
	List< Value_weak_ptr<IResource_Material>>& GetMaterial() { return list_vwp_material; }
	const List< Value_weak_ptr<IResource_Material>>& GetMaterial() const { return list_vwp_material; }
	void SetModel(Value_weak_ptr<IResource_Model> arg_vwp_model) { vwp_model = arg_vwp_model; }
	void SetMesh(Value_weak_ptr<IResource_Mesh> arg_vwp_mesh) { vwp_mesh = arg_vwp_mesh; }
	void SetMaterial(const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material) { list_vwp_material = arg_list_vwp_material; }
private:
	std::uint32_t cBufferCount = 0;
	Value_ptr < CBuffer<Matrices>> cbuffer;
	std::uint32_t* p_collisionRegistIndex;
	Value_weak_ptr<IResource_Model> vwp_model;
	Value_weak_ptr<IResource_Mesh> vwp_mesh;
	List< Value_weak_ptr<IResource_Material>> list_vwp_material;
};

class DrawObject :public IDrawObject {
public:
	virtual void SetTransform(const Value_ptr<Transform>& arg_transform) = 0;
	virtual bool IsCreated() = 0;
	BUTIRENDERING_API void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
	BUTIRENDERING_API std::uint32_t* GetOctRegistPtr() override;
	DrawData& GetDrawData() { return drawData; }
	const DrawData& GetDrawData()const { return drawData; }
	virtual Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
		return drawData.AddICBuffer(arg_cbuffer);
	}
	virtual inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {
		return drawData.GetICBuffer(arg_bufferName);
	}
protected:
	DrawData drawData;
};
class DualDrawObject :public DrawObject {
public:
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void PreInitialize() override;
	BUTIRENDERING_API void Draw()override;
	BUTIRENDERING_API void DrawBefore() override;
	BUTIRENDERING_API float GetZ(const Matrix4x4& arg_vpMatrix)override;
	BUTIRENDERING_API void SetInfo() override;
	BUTIRENDERING_API void SetInfo_WithoutCommand() override;
	BUTIRENDERING_API void BufferUpdate() override;
	BUTIRENDERING_API void CommandSet() override;
	BUTIRENDERING_API void CommandExecute() override;
	BUTIRENDERING_API void SetTransform(const Value_ptr<Transform>& arg_transform)override;
	BUTIRENDERING_API void SetOctRegistPtr(std::uint32_t* arg_ptr) override;
	bool IsCreated()override {
		return vlp_afterDrawObj->IsCreated() && vlp_befDrawObj->IsCreated();
	}
	BUTIRENDERING_API std::uint32_t* GetOctRegistPtr() override;
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)override {
		auto end = vlp_befDrawObj->GetDrawData().vlp_drawInfo->vec_exCBuffer.end();
		for (auto itr = vlp_befDrawObj->GetDrawData().vlp_drawInfo->vec_exCBuffer.begin(); itr != end; itr++) {
			if ((*itr)->GetExName() == arg_bufferName) {
				return *itr;
			}
		}

		return nullptr;
	}
	Value_ptr<DrawObject> vlp_befDrawObj;
	Value_ptr<DrawObject> vlp_afterDrawObj;
};
class IModelObject {
public:
	virtual Value_ptr<IBoneObject>& GetModelBone() = 0;
	virtual Value_ptr< CBuffer<BoneMatrix>>& GetBoneCBuffer() = 0;
};
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_weak_ptr<IResource_Model>& arg_vwp_model, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);

BUTIRENDERING_API Value_ptr<IDrawObject> CreateModelDrawObject(const Value_weak_ptr<IResource_Model>& arg_vwp_model,Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateModelDrawObject(const Value_weak_ptr<IResource_Model>& arg_vwp_model,const List<Value_weak_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);

}
}

#endif // !IDrawData_H