#pragma once
#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include "ButiRendering_Dx12/Header/DrawData/DrawData_Dx12.h"

ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelDrawObject_Dx12(const Value_ptr<IResource_Model>& arg_vwp_model,  Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone)
:ModelDrawObject_Dx12(arg_vwp_model, arg_vwp_model->GetMaterial(), arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform, arg_vlp_bone) {
}

ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelDrawObject_Dx12(const Value_ptr<IResource_Model>& arg_vwp_model,  const List<Value_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<ObjectDrawData> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone)
	: DrawObject_Dx12(DrawData(arg_vlp_transform))
{
	vwp_graphicDevice = arg_vwp_graphicDevice;
	drawData.vlp_renderer = arg_vlp_renderer;
	drawData.SetModel(arg_vwp_model);
	drawData.SetMesh(arg_vwp_model->GetMesh());
	drawData.SetMaterial( arg_list_material);
	drawData.subset = arg_vwp_model->GetSubset();
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	vlp_bone = arg_vlp_bone;
}

void ButiEngine::ButiRendering::ModelDrawObject_Dx12::Initialize()
{
	DrawObject_Dx12::Initialize();
	cbuffer_bone = drawData.GetCBuffer<BoneMatrix>();
}

void ButiEngine::ButiRendering::ModelDrawObject_Dx12::DrawBefore()
{
	drawData.transform = drawData.vlp_transform->ToMatrix();
	ModelUpdate();
}


void ButiEngine::ButiRendering::ModelDrawObject_Dx12::Draw()
{
	BufferUpdate();
	CommandSet();
}


void ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelUpdate()
{
	std::int32_t index = 0;	
	if(cbuffer_bone)
	for (auto itr :vlp_bone-> m_list_bone) {
		auto mat = (itr)->GetPoseMatrix();
		cbuffer_bone->Get().datas[index] = mat;
		index++;
	}

}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateModelDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone) {
	return ObjectFactory::Create<ModelDrawObject_Dx12>(arg_vwp_model, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(), arg_vlp_drawInfo, arg_vlp_transform, arg_vlp_bone);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IDrawObject> ButiEngine::ButiRendering::CreateModelDrawObject(const Value_ptr<IResource_Model>& arg_vwp_model,const List<Value_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice, Value_ptr< ObjectDrawData >arg_vlp_drawInfo, Value_ptr<Transform>arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone) {
	return ObjectFactory::Create<ModelDrawObject_Dx12>(arg_vwp_model, arg_list_material, arg_vlp_renderer, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>(), arg_vlp_drawInfo, arg_vlp_transform, arg_vlp_bone);
}