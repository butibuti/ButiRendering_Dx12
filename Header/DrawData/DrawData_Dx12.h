#pragma once
#include"stdafx.h"
#include"IDrawData.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CBuffer_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CArrayBuffer_Dx12.h"
namespace ButiEngine {
namespace ButiRendering {
class DrawObject_Dx12;

using MatrixUpdateFunc = void (DrawObject_Dx12::*)();

class DrawObject_Dx12 :public DrawObject {
public:
	BUTIRENDERING_API void Initialize(const std::uint32_t srvCount);

	BUTIRENDERING_API void CreatePipeLineState(const std::uint32_t arg_exCBuffer, const std::uint32_t srvCount);
	BUTIRENDERING_API void CommandExecute();
	BUTIRENDERING_API void BufferUpdate()override;
	BUTIRENDERING_API void SetInfo();
	BUTIRENDERING_API void SetInfo_WithoutCommand();
	BUTIRENDERING_API void CommandSet();
	BUTIRENDERING_API Value_ptr<ICBuffer> AddICBuffer(Value_ptr<ICBuffer> arg_cbuffer)override;
	BUTIRENDERING_API bool IsCreated()override;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

	std::vector< D3D12_GPU_DESCRIPTOR_HANDLE >vec_samplerBufferDescriptorHandle;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	D3D12_RASTERIZER_DESC rasterizerStateDesc;
	D3D12_ROOT_SIGNATURE_DESC rootdesc;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::int32_t textureRegion;

	MatrixUpdateFunc p_matrixUpdateFunc;

private:
	void MatrixUpdater_NoRotation();
	void MatrixUpdater_RemoveDecimalPart();
	void MatrixUpdater_RemoveDecimalPartXY();
	void MatrixUpdater_RemoveDecimalPartYZ();
	void MatrixUpdater_RemoveDecimalPartXZ();
	void MatrixUpdater_default();
	void MatrixUpdater_billBoard();
	void MatrixUpdater_billBoardX();
	void MatrixUpdater_billBoardY();
	void MatrixUpdater_billBoardZ();

};

class MeshDrawObject_Dx12 :public DrawObject_Dx12 {
public:
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const Value_weak_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);

	BUTIRENDERING_API void DrawBefore()override;
	BUTIRENDERING_API void Draw()override;

	void SetTransform(const Value_ptr<Transform>& arg_transform)override {
		drawData.transform = arg_transform->GetMatrix();
		drawData.vlp_transform = arg_transform;
	}

	inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
		return  drawData.GetMaxZ(arg_vpMatrix);
	}
	void ShowZ() override {}

	BUTIRENDERING_API void Initialize()override;
};

class ModelDrawObject_Dx12 :public DrawObject_Dx12, public IModelObject {
public:
	BUTIRENDERING_API ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_shader, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);
	BUTIRENDERING_API ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_shader, const List<Value_weak_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);

	inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
		return  drawData.GetMaxZ(arg_vpMatrix);
	}
	BUTIRENDERING_API void Initialize()override;


	BUTIRENDERING_API void DrawBefore()override;

	void SetTransform(const Value_ptr<Transform>& arg_transform)override {
		drawData.transform = arg_transform->GetMatrix();
		drawData.vlp_transform = arg_transform;
	}
	Value_ptr<IBoneObject>& GetModelBone() override {
		return vlp_bone;
	}
	Value_ptr< CArrayBuffer<Matrix4x4>>& GetBoneCBuffer() override {
		return cbuffer_bone;
	}

	BUTIRENDERING_API void Draw()override;

	BUTIRENDERING_API void  ModelUpdate();

	Value_ptr<IBoneObject> vlp_bone;
	Value_ptr< CArrayBuffer<Matrix4x4>> cbuffer_bone;
};
}
}
