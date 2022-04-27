#pragma once
#include"stdafx.h"
#include"../../Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"Header/GameParts/ResourceContainer.h"
#include "../../Header/DrawData/DrawData_Dx12.h"

ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_vwp_shader, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone)
:ModelDrawObject_Dx12(arg_vwp_model, arg_vwp_shader, arg_vwp_model.lock()->GetMaterial(), arg_vlp_renderer, arg_vwp_graphicDevice, arg_vlp_drawInfo, arg_vlp_transform, arg_vlp_bone) {
}

ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelDrawObject_Dx12(const Value_weak_ptr<IResource_Model>& arg_vwp_model, const Value_weak_ptr<IResource_Shader>& arg_shader, const List<Value_weak_ptr<IResource_Material>>& arg_list_material, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, Value_ptr<DrawInformation> arg_vlp_drawInfo, Value_ptr<Transform> arg_vlp_transform, Value_ptr<IBoneObject> arg_vlp_bone)
{
	vwp_graphicDevice = arg_vwp_graphicDevice;
	drawData.vlp_renderer = arg_vlp_renderer;
	drawData.SetModel(arg_vwp_model);
	drawData.SetMesh(arg_vwp_model.lock()->GetMesh());
	drawData.SetMaterial( arg_list_material);
	drawData.SetShader(arg_shader);
	drawData.subset = arg_vwp_model.lock()->GetSubset();
	drawData.vlp_drawInfo = arg_vlp_drawInfo;
	drawData.transform = arg_vlp_transform->GetMatrix();
	drawData.vlp_transform = arg_vlp_transform;
	vlp_bone = arg_vlp_bone;
}

void ButiEngine::ButiRendering::ModelDrawObject_Dx12::Initialize()
{
	

	cbuffer_bone = drawData.GetCArrayBuffer<Matrix4x4>("bone");

	//ModelUpdate();




	std::int32_t srvCount = 0;
	auto container = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer();
	for (std::int32_t i = 0; i < drawData.GetMaterial().GetSize(); i++) {
		auto textureCount = drawData.GetMaterial()[i].lock()->GetTextureCount();
		srvCount = max(srvCount, textureCount);
	}

	textureRegion = srvCount;
	//DrawObject_Dx12::Initialize(srvCount);


}

void ButiEngine::ButiRendering::ModelDrawObject_Dx12::DrawBefore()
{

	drawData.transform = drawData.vlp_transform->ToMatrix();
	ModelUpdate();
}


void ButiEngine::ButiRendering::ModelDrawObject_Dx12::Draw()
{
	BufferUpdate();
	//CommandExecute();
}


void ButiEngine::ButiRendering::ModelDrawObject_Dx12::ModelUpdate()
{
	std::int32_t i = 0;
	
	for (auto itr =vlp_bone-> vec_bone.begin(), end = vlp_bone->vec_bone.end(); itr !=end ; itr++, i++) {
		cbuffer_bone->Get(i) = (*itr)->GetPoseMatrix();
	}
}
