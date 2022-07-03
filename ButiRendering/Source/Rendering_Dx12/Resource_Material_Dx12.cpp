#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12\Header\CBuffer.h"
#include "..\..\..\Header\Rendering_Dx12\Resource_Material_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/RootSignatureManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"
ButiEngine::ButiRendering::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialValue & arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice) : Resource_Material(arg_var)
{
	materialVar = arg_var;
	list_vwp_texture = arg_list_texture;
	vwp_graphicDevice = arg_vwp_graphicDevice;
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = CreateCBuffer <MaterialValue>(1, "Material");
	materialBuffer_Dx12->SetGraphicDevice(vwp_graphicDevice.lock());
	materialBuffer_Dx12->Get().ambient = GetMaterialVariable().ambient;
	materialBuffer_Dx12->Get().emissive = GetMaterialVariable().emissive;
	materialBuffer_Dx12->Get().diffuse = GetMaterialVariable().diffuse;
	materialBuffer_Dx12->Get().specular = GetMaterialVariable().specular;
	materialBuffer_Dx12->CreateBuffer();
	materialBuffer = materialBuffer_Dx12;
	vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->RegistUpdateListner(GetThis<IDescriptorHeapUpdateListner>());
	std::vector< D3D12_GPU_DESCRIPTOR_HANDLE >vec_samplerBufferDescriptorHandle;
	for (auto sampleMode: GetDrawSettings().vec_samplerMode) {
		vec_samplerBufferDescriptorHandle.push_back(vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHandle(static_cast<std::int32_t>(sampleMode)).GPUHandle);
	}
	auto vlp_rootSignature = vwp_graphicDevice.lock()->GetRootSignatureManager().GetRootSignature(GetTextureCount() + m_drawSetting.isShadowMap, m_vlp_shader->GetConstantBufferCount(),vec_samplerBufferDescriptorHandle);
	m_vlp_pipelineState = vwp_graphicDevice.lock()->GetPipelineStateManager().GetPipelineState(m_drawSetting, vlp_rootSignature, m_vlp_shader);
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_vlp_renderer)
{
	for (std::int32_t i = 0; i < list_vwp_texture.GetSize(); i++) {
		list_vwp_texture[i].lock()->Attach(i);
	}
	materialBuffer->Attach(slotOffset);
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Update()
{
	materialBuffer->Update();
}

std::uint32_t ButiEngine::ButiRendering::Resource_Material_Dx12::GetTextureCount() const
{
	return list_vwp_texture.GetSize();
}


void ButiEngine::ButiRendering::Resource_Material_Dx12::SetMaterialVariable(const MaterialValue& arg_var)
{
	materialBuffer->Get() = arg_var;
}

ButiEngine::ButiRendering::MaterialValue& ButiEngine::ButiRendering::Resource_Material_Dx12::GetMaterialVariable()
{
	if(materialBuffer)
	return materialBuffer->Get();
	else {
		return materialVar;
	}
}

const  ButiEngine::ButiRendering::MaterialValue& ButiEngine::ButiRendering::Resource_Material_Dx12::GetMaterialVariable() const
{
	return materialVar;
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::SetMaterialIndex(const std::int32_t arg_index)
{
	materialBuffer->Get().materialID =(float) arg_index+0.1;
	Update();
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::OnDescriptorHeapUpdate()
{
	Update();
}

ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Resource_MaterialList_Dx12(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:Resource_Material(arg_var)
{
	materialVar = arg_var;
	list_vwp_texture = arg_list_texture;
	vwp_graphicDevice = arg_vwp_graphicDevice;
	m_list_material = arg_list_material;
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = CreateCBuffer<MaterialValue_Deferred>(1,"MatrialList");
	materialBuffer_Dx12->SetGraphicDevice(vwp_graphicDevice.lock());
	materialBuffer_Dx12->CreateBuffer();
	materialBuffer = materialBuffer_Dx12;
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_vlp_renderer)
{
	
	for (std::int32_t i = 0; i < list_vwp_texture.GetSize(); i++) {
		list_vwp_texture[i].lock()->Attach(i);
	}
	materialBuffer->Attach(slotOffset);
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Update()
{
	std::int32_t i = 0;
	for (auto itr :m_list_material) {
		materialBuffer->Get().datas[i] = itr->GetMaterialVariable();
		i++;
	}
	materialBuffer->Update();
}

std::uint32_t ButiEngine::ButiRendering::Resource_MaterialList_Dx12::GetTextureCount() const
{
	return list_vwp_texture.GetSize();
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::SetMaterialVariable(const MaterialValue& arg_var)
{
	materialVar = arg_var;
}

ButiEngine::ButiRendering::MaterialValue& ButiEngine::ButiRendering::Resource_MaterialList_Dx12::GetMaterialVariable()
{
	return materialVar;
}

const  ButiEngine::ButiRendering::MaterialValue& ButiEngine::ButiRendering::Resource_MaterialList_Dx12::GetMaterialVariable() const
{
	return materialVar;
}
void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::SetMaterialIndex(const std::int32_t arg_index)
{
	materialVar.materialID = arg_index;
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::SetMaterialList(const List<Value_ptr<IResource_Material>>& arg_list_material)
{
	m_list_material = arg_list_material;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterial(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, Value_weak_ptr<IResource_Texture> arg_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return CreateMaterial(arg_var,arg_vlp_shader, { arg_texture }, arg_vwp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterial(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List<Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return ObjectFactory::Create<ButiRendering::Resource_Material_Dx12>(arg_var, arg_list_texture, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterialList(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List<Value_weak_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return ObjectFactory::Create<ButiRendering::Resource_MaterialList_Dx12>(arg_var, arg_list_texture, arg_list_material,arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
}

