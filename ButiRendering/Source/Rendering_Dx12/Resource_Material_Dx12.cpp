#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12\Header\CBuffer.h"
#include "..\..\..\Header\Rendering_Dx12\Resource_Material_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/RootSignatureManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"
ButiEngine::ButiRendering::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialValue & arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_ptr<IResource_Texture>>& arg_list_texture, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	: Resource_Material(arg_var, arg_vlp_shader,arg_list_texture,arg_drawSettings),  m_vwp_graphicDevice(arg_vwp_graphicDevice) {}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = CreateCBuffer <MaterialValue>();
	materialBuffer_Dx12->SetGraphicDevice(m_vwp_graphicDevice.lock());
	materialBuffer_Dx12->Get()= materialVar;
	materialBuffer_Dx12->CreateBuffer();
	m_materialBuffer = materialBuffer_Dx12;
	m_vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->RegistUpdateListner(GetThis<IDescriptorHeapUpdateListner>());
	std::int32_t samplerCountGap = m_vlp_shader->GetSamplerReflectionDatas().GetSize() - GetDrawSettings().vec_samplerMode.size();
	if (samplerCountGap>0){
		for (std::int32_t i =samplerCountGap ; i > 0; i--) {
			GetDrawSettings().vec_samplerMode.push_back(SamplerState::LinearClamp);
		}
	}
	else if (samplerCountGap<0) {
		for (std::int32_t i =samplerCountGap; i < 0; i++) {
			GetDrawSettings().vec_samplerMode.pop_back();
		}
	}
	auto vlp_rootSignature = m_vwp_graphicDevice.lock()->GetRootSignatureManager().GetRootSignature(m_vlp_shader);
	m_vlp_pipelineState = m_vwp_graphicDevice.lock()->GetPipelineStateManager().GetPipelineState(m_drawSetting, vlp_rootSignature, m_vlp_shader);
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Attach()
{
	std::int32_t index = 0;
	for (auto sampler: GetDrawSettings().vec_samplerMode) {
		m_vwp_graphicDevice.lock()->SetSamplerState(sampler, index+m_list_vlp_texture.GetSize());
		index++;
	}
	index=0;
	for (auto texture : m_list_vlp_texture) {
		m_vwp_graphicDevice.lock()->SetTexture(texture, index);
		index++;
	}
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Update()
{
	m_materialBuffer->Update();
}

std::uint32_t ButiEngine::ButiRendering::Resource_Material_Dx12::GetTextureCount() const
{
	return m_list_vlp_texture.GetSize();
}


void ButiEngine::ButiRendering::Resource_Material_Dx12::SetMaterialVariable(const MaterialValue& arg_var)
{
	m_materialBuffer->Get() = arg_var;
}

ButiEngine::ButiRendering::MaterialValue& ButiEngine::ButiRendering::Resource_Material_Dx12::GetMaterialVariable()
{
	if(m_materialBuffer)
	return m_materialBuffer->Get();
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
	m_materialBuffer->Get().materialID =(float) arg_index+0.1;
	Update();
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::OnDescriptorHeapUpdate()
{
	Update();
}

ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Resource_MaterialList_Dx12(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:Resource_Material(arg_var,arg_vlp_shader,arg_list_texture,arg_drawSettings)	, m_vwp_graphicDevice(arg_vwp_graphicDevice) {}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = CreateCBuffer<MaterialValue_Deferred>();
	materialBuffer_Dx12->SetGraphicDevice(m_vwp_graphicDevice.lock());
	materialBuffer_Dx12->CreateBuffer();
	m_materialBuffer = materialBuffer_Dx12; 
	auto vlp_rootSignature = m_vwp_graphicDevice.lock()->GetRootSignatureManager().GetRootSignature(m_vlp_shader);
	m_vlp_pipelineState = m_vwp_graphicDevice.lock()->GetPipelineStateManager().GetPipelineState(m_drawSetting, vlp_rootSignature, m_vlp_shader);
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Attach()
{
	std::int32_t index = 0;
	for (auto sampler : GetDrawSettings().vec_samplerMode) {
		m_vwp_graphicDevice.lock()->SetSamplerState(sampler, index + m_list_vlp_texture.GetSize());
		index++;
	}
	index = 0;
	for (auto texture : m_list_vlp_texture) {
		m_vwp_graphicDevice.lock()->SetTexture(texture, index);
		index++;
	}
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Update()
{
	std::int32_t i = 0;
	for (auto itr :m_list_material) {
		m_materialBuffer->Get().datas[i] = itr->GetMaterialVariable();
		i++;
	}
	m_materialBuffer->Update();
}

std::uint32_t ButiEngine::ButiRendering::Resource_MaterialList_Dx12::GetTextureCount() const
{
	return m_list_vlp_texture.GetSize();
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
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterial(const std::string& arg_materialName, const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List<Value_ptr<IResource_Texture>>& arg_list_texture, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	auto material = ObjectFactory::Create<ButiRendering::Resource_Material_Dx12>(arg_var, arg_vlp_shader, arg_list_texture, arg_drawSettings, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());;
	material->SetMaterialName(arg_materialName);
	return material;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterialList(const std::string& arg_materialName, const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List<Value_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	auto material= ObjectFactory::Create<ButiRendering::Resource_MaterialList_Dx12>(arg_var, arg_vlp_shader, arg_list_texture, arg_list_material, arg_drawSettings, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	material->SetMaterialName(arg_materialName);
	return material;
}

