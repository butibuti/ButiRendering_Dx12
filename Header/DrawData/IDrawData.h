#ifndef IDrawData_H
#define IDrawData_H
#pragma once
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
#include"../CBuffer.h"
#include"IDrawObject.h"
namespace ButiEngine {

namespace ButiRendering {
struct ObjectDrawData :public IObject {
	ObjectDrawData() {}
	std::vector<Value_ptr<ICBuffer>> vec_exCBuffer;
	DrawFixParam drawFixParam = DrawFixParam::none;
	BillBoardMode billboardMode = BillBoardMode::none;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(vec_exCBuffer);
		ARCHIVE_BUTI(billboardMode);
		ARCHIVE_BUTI(drawFixParam);
	}
	bool IsContainExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
			if ((*itr)->GetBufferName() == arg_cbufferName) {
				return true;
			}
		}
		return false;
	}
	void RemoveExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
			if ((*itr)->GetBufferName() == arg_cbufferName) {
				vec_exCBuffer.erase(itr);
				return;
			}
			else
				itr++;
		}
	}
	Value_ptr<ICBuffer> GetExCBuffer(const std::string& arg_cbufferName) {
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
			if ((*itr)->GetBufferName() == arg_cbufferName) {
				return *itr;
			}
			else {
				itr++;
			}
		}
		return nullptr;
	}

	Value_ptr< ObjectDrawData > Clone() {
		auto output = ObjectFactory::Create<ObjectDrawData>();
		output->billboardMode = billboardMode;
		output->drawFixParam= drawFixParam;
		for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
			output->vec_exCBuffer.push_back((*itr)->Clone());
		}

		return output;
	}
};
class DrawData {
public:
	DrawData(){}
	DrawData(Value_ptr<Transform> arg_vlp_transform):vlp_transform(arg_vlp_transform?arg_vlp_transform:ObjectFactory::Create<Transform>())
	{transform = (vlp_transform->GetMatrix());}
	Matrix4x4 transform;
	Value_ptr<Transform> vlp_transform;
	bool m_isAlphaDraw = false;
	std::vector<std::uint32_t> subset;
	Value_ptr< ObjectDrawData >vlp_drawInfo;
	Value_ptr<IRenderer> vlp_renderer;
	inline float GetMaxZ(const Matrix4x4& arg_viewMatrix) {
		auto viewPos_dlb = vwp_mesh.lock()->GetBoxEightCorner().down_left_back * (arg_viewMatrix*transform).Transpose();
		auto viewPos_drb = vwp_mesh.lock()->GetBoxEightCorner().down_right_back * (arg_viewMatrix*transform).Transpose();
		auto viewPos_dlf = vwp_mesh.lock()->GetBoxEightCorner().down_left_front * (arg_viewMatrix*transform).Transpose();
		auto viewPos_drf = vwp_mesh.lock()->GetBoxEightCorner().down_right_front * (arg_viewMatrix*transform).Transpose();
		auto viewPos_ulb = vwp_mesh.lock()->GetBoxEightCorner().up_left_back * (arg_viewMatrix*transform).Transpose();
		auto viewPos_urb = vwp_mesh.lock()->GetBoxEightCorner().up_right_back * (arg_viewMatrix*transform).Transpose();
		auto viewPos_ulf = vwp_mesh.lock()->GetBoxEightCorner().up_left_front * (arg_viewMatrix*transform).Transpose();
		auto viewPos_urf = vwp_mesh.lock()->GetBoxEightCorner().up_right_front * (arg_viewMatrix*transform).Transpose();
		return max(max(max(viewPos_dlb.z/ viewPos_dlb.w, viewPos_drb.z/ viewPos_drb.w), 
			max(viewPos_dlf.z/ viewPos_dlf.w, viewPos_drf.z/ viewPos_drf.w)),
			max(max(viewPos_ulb.z/ viewPos_ulb.w, viewPos_urb.z/ viewPos_urb.w), 
			max(viewPos_ulf.z/ viewPos_ulf.w, viewPos_urf.z/ viewPos_urf.w)));
	}
	inline Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer) {
		vlp_drawInfo->vec_exCBuffer.push_back(arg_cbuffer);
		return arg_cbuffer;
	}
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)const {

		for (auto itr : vlp_drawInfo->vec_exCBuffer) {
			if ((itr)->GetBufferName() == arg_bufferName) {
				return itr;
			}
		}

		return nullptr;
	}
	inline std::vector<Value_ptr<ICBuffer>>& GetICBuffers() {
		return  vlp_drawInfo->vec_exCBuffer;
	}
	inline const std::vector<Value_ptr<ICBuffer>>& GetICBuffers()const {
		return  vlp_drawInfo->vec_exCBuffer;
	}

	template <class T>
	inline Value_ptr<CBuffer<T>> GetCBuffer()const {
		auto out = GetICBuffer(T::GetConstantBufferName());
		if (out && out->IsThis<CBuffer<T>>()) {
			return out->GetThis<CBuffer<T>>();
		}

		return nullptr;
	}
	inline void RemoveCBuffer(const std::string& arg_bufferName) {
		for (auto itr = vlp_drawInfo->vec_exCBuffer.begin(); itr != vlp_drawInfo->vec_exCBuffer.end(); itr++) {
			if ((*itr)->GetBufferName() == arg_bufferName) {
				vlp_drawInfo->vec_exCBuffer.erase(itr);
				return;
			}
		}
	}
	template <class T>
	inline void RemoveCBuffer(){
		RemoveCBuffer(T::GetConstantBufferName());
	}
	Value_ptr < CBuffer<Matrices>>& GetCBuffer() { return cbuffer; }
	void SetCbuffer(const Value_ptr < CBuffer<Matrices>>& arg_cBuffer) { cbuffer = arg_cBuffer; }
	std::uint32_t* GetCollisionIndex() { return p_collisionRegistIndex; }
	void SetCollisionIndex(std::uint32_t* arg_p_collisionIndex) { p_collisionRegistIndex = arg_p_collisionIndex; }
	std::uint32_t GetCBufferCount()const { return cBufferCount; }
	void SetCBufferCount(const std::uint32_t arg_count) { cBufferCount = arg_count; }

	Value_weak_ptr<IResource_Model> GetModel() const { return vwp_model; }
	Value_weak_ptr<IResource_Mesh> GetMesh()const { return vwp_mesh; }
	List< Value_ptr<IResource_Material>>& GetMaterial() { return list_vwp_material; }
	const List< Value_ptr<IResource_Material>>& GetMaterial() const { return list_vwp_material; }
	void SetModel(Value_weak_ptr<IResource_Model> arg_vwp_model) { vwp_model = arg_vwp_model; }
	void SetMesh(Value_weak_ptr<IResource_Mesh> arg_vwp_mesh) { vwp_mesh = arg_vwp_mesh; }
	void SetMaterial(const List< Value_ptr<IResource_Material>>& arg_list_vwp_material) { list_vwp_material = arg_list_vwp_material; }
private:
	std::uint32_t cBufferCount = 0;
	Value_ptr < CBuffer<Matrices>> cbuffer;
	std::uint32_t* p_collisionRegistIndex;
	Value_weak_ptr<IResource_Model> vwp_model;
	Value_weak_ptr<IResource_Mesh> vwp_mesh;
	List< Value_ptr<IResource_Material>> list_vwp_material;
};

class DrawObject :public IDrawObject {
public:
	DrawObject(){}
	DrawObject(const DrawData& arg_data):drawData(arg_data){}
	virtual bool IsCreated() = 0;
	DrawData& GetDrawData() { return drawData; }
	const DrawData& GetDrawData()const { return drawData; }
	inline Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)const override {
		return drawData.GetICBuffer(arg_bufferName);
	}
	inline std::vector<Value_ptr<ICBuffer>>& GetICBuffers() override {
		return drawData.GetICBuffers();
	}
	inline const std::vector<Value_ptr<ICBuffer>>& GetICBuffers() const override {
		return drawData.GetICBuffers();
	}

	void CreateBuffer()override {
		drawData.GetCBuffer()->CreateBuffer();
		for (auto cbuffer : GetICBuffers()) {
			cbuffer->CreateBuffer();
		}
	}
	List<Value_ptr<IDrawCommand>>& GetCommands() { return m_list_command; }
	const List<Value_ptr<IDrawCommand>>& GetCommands()const {return m_list_command;}
	bool IsAlphaObject()const override { return drawData.m_isAlphaDraw; }
	Value_ptr<Transform> GetTransform()const { return drawData.vlp_transform; }
	void SetTransform(const Value_ptr<Transform> arg_vlp_transform) { drawData.vlp_transform = arg_vlp_transform; }
protected:
	DrawData drawData;
	List<Value_ptr<IDrawCommand>> m_list_command;
};
class DualDrawObject :public DrawObject {
public:
	DualDrawObject(Value_ptr<DrawObject> arg_vlp_befDrawObj, Value_ptr<DrawObject> arg_vlp_afterDrawObj):m_vlp_befDrawObj(arg_vlp_befDrawObj),m_vlp_afterDrawObj(arg_vlp_afterDrawObj){}
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void Draw()override;
	BUTIRENDERING_API void DrawBefore() override;
	BUTIRENDERING_API float GetZ(const Matrix4x4& arg_vpMatrix)override;
	BUTIRENDERING_API void SetInfo() override;
	BUTIRENDERING_API void SetInfo_WithoutCommand() override;
	BUTIRENDERING_API void BufferUpdate() override;
	BUTIRENDERING_API void CommandSet() override;
	BUTIRENDERING_API void CommandExecute() override;
	BUTIRENDERING_API void SetTransform(Value_ptr<Transform> arg_transform)override;
	Value_ptr<Transform> GetTransform()const { return m_vlp_befDrawObj->GetTransform(); }
	bool IsCreated()override {
		return m_vlp_afterDrawObj->IsCreated() && m_vlp_befDrawObj->IsCreated();
	}
	Value_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName)const override {
		for (auto itr :m_vlp_befDrawObj->GetDrawData().vlp_drawInfo->vec_exCBuffer) {
			if ((itr)->GetBufferName() == arg_bufferName) {
				return itr;
			}
		}
		return nullptr;
	}

	void CreateBuffer() override{
		m_vlp_befDrawObj->CreateBuffer();
		m_vlp_afterDrawObj->CreateBuffer();
	}
	std::vector<Value_ptr<ICBuffer>>& GetICBuffers() override {
		return m_vlp_befDrawObj->GetDrawData().GetICBuffers();
	}
	const std::vector<Value_ptr<ICBuffer>>& GetICBuffers() const override {
		return m_vlp_befDrawObj->GetDrawData().GetICBuffers();
	}
private:
	Value_ptr<DrawObject> m_vlp_befDrawObj,m_vlp_afterDrawObj;
};
class IModelObject {
public:
	virtual Value_ptr<IBoneObject>& GetModelBone() = 0;
	virtual Value_ptr< CBuffer<BoneMatrix>>& GetBoneCBuffer() = 0;
};
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_ptr<IResource_Mesh>& arg_vwp_mesh, const List< Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateMeshDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model, const List< Value_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);

BUTIRENDERING_API Value_ptr<IDrawObject> CreateModelDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model,Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);
BUTIRENDERING_API Value_ptr<IDrawObject> CreateModelDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model,const List<Value_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);

}
}

#endif // !IDrawData_H