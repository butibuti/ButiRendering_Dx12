#ifndef DRAWDATA_DX12_H
#define DRAWDATA_DX12_H
#pragma once
#include"stdafx.h"
#include"IDrawData.h"
#include"../Rendering_Dx12/GraphicDevice_Dx12.h"
#include<d3dx12.h>
namespace ButiEngine {
namespace ButiRendering {
class DrawObject_Dx12;
using MatrixUpdateFunc = void (DrawObject_Dx12::*)();

class DrawObject_Dx12 :public DrawObject {
public:
	DrawObject_Dx12() {}
	DrawObject_Dx12(const DrawData& arg_data) :DrawObject(arg_data) {}
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void BufferUpdate()override;
	BUTIRENDERING_API void SetInfo();
	BUTIRENDERING_API void CommandExecute();
	BUTIRENDERING_API bool IsCreated()override;

protected:
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
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
	MatrixUpdateFunc p_matrixUpdateFunc;
};

class MeshDrawObject_Dx12 :public DrawObject_Dx12 {
public:
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const Value_weak_ptr<IResource_Material>& arg_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Mesh>& arg_vwp_mesh, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);
	BUTIRENDERING_API MeshDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const List< Value_weak_ptr<IResource_Material>>& arg_list_vwp_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform);

	BUTIRENDERING_API void DrawBefore()override;
	BUTIRENDERING_API void Draw()override;

	void SetTransform(Value_ptr<Transform> arg_transform)override {
		drawData.transform = arg_transform->GetMatrix();
		drawData.vlp_transform = arg_transform;
	}

	inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
		return  drawData.GetMaxZ(arg_vpMatrix);
	}
	void ShowZ() override {}

};

class ModelDrawObject_Dx12 :public DrawObject_Dx12, public IModelObject {
public:
	BUTIRENDERING_API ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model,Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);
	BUTIRENDERING_API ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model,const List<Value_weak_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr< DrawInformation >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone);

	inline float GetZ(const Matrix4x4& arg_vpMatrix)override {
		return  drawData.GetMaxZ(arg_vpMatrix);
	}
	BUTIRENDERING_API void Initialize()override;


	BUTIRENDERING_API void DrawBefore()override;

	void SetTransform(Value_ptr<Transform> arg_transform)override {
		drawData.transform = arg_transform->GetMatrix();
		drawData.vlp_transform = arg_transform;
	}
	Value_ptr<IBoneObject>& GetModelBone() override {
		return vlp_bone;
	}
	Value_ptr< CBuffer<BoneMatrix>>& GetBoneCBuffer() override {
		return cbuffer_bone;
	}

	BUTIRENDERING_API void Draw()override;

	BUTIRENDERING_API void  ModelUpdate();

	Value_ptr<IBoneObject> vlp_bone;
	Value_ptr< CBuffer<BoneMatrix>> cbuffer_bone;
};
}
}
#endif