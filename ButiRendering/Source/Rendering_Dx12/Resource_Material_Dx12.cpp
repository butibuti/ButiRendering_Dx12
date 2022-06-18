#include "stdafx.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Material_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/CArrayBuffer_Dx12.h"
#include"ButiRendering_Dx12\Header\Rendering_Dx12\Resource_Material_Dx12.h"
#include "ButiRendering_Dx12\Header\Rendering_Dx12\Resource_Material_Dx12.h"
#include "..\..\..\Header\Rendering_Dx12\Resource_Material_Dx12.h"

ButiEngine::ButiRendering::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialValue & arg_var, Value_weak_ptr<IResource_Texture> arg_texture, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice) :Resource_Material(arg_var)
{
	materialVar = arg_var;
	list_vwp_texture.Add( arg_texture);
	vwp_graphicDevice = arg_vwp_graphicDevice;
}

ButiEngine::ButiRendering::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialValue & arg_var, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice) : Resource_Material(arg_var)
{
	materialVar = arg_var;
	list_vwp_texture = arg_list_texture;
	vwp_graphicDevice = arg_vwp_graphicDevice;
}

void ButiEngine::ButiRendering::Resource_Material_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = CreateCBuffer <MaterialValue>(1,"Material");
	materialBuffer_Dx12->SetGraphicDevice(vwp_graphicDevice.lock());

	materialBuffer_Dx12->Get().ambient = GetMaterialVariable().ambient;
	materialBuffer_Dx12->Get().emissive = GetMaterialVariable().emissive;
	materialBuffer_Dx12->Get().diffuse = GetMaterialVariable().diffuse;
	materialBuffer_Dx12->Get().specular = GetMaterialVariable().specular;
	materialBuffer_Dx12->CreateBuffer();
	materialBuffer = materialBuffer_Dx12;
	vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->RegistUpdateListner(GetThis<IDescriptorHeapUpdateListner>());
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

ButiEngine::ButiRendering::MaterialValue_Deferred ButiEngine::ButiRendering::Resource_Material_Dx12::GetMaterialDeferredValue()const
{
	MaterialValue_Deferred output;
	output.ambient = materialBuffer->Get().ambient;
	output.diffuse = materialBuffer->Get().diffuse;
	output.emissive = materialBuffer->Get().emissive;
	output.specular = materialBuffer->Get().specular;
	return output;
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

ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Resource_MaterialList_Dx12(const MaterialValue& arg_var, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:Resource_Material(arg_var)
{
	materialVar = arg_var;
	list_vwp_texture = arg_list_texture;
	vwp_graphicDevice = arg_vwp_graphicDevice;
	m_list_material = arg_list_material;
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = ObjectFactory::Create<CArrayBuffer_Dx12<MaterialValue_Deferred>>(1,256);
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
		materialBuffer->Get(i) = MaterialValue_Deferred( itr->GetMaterialVariable());
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

ButiEngine::ButiRendering::MaterialValue_Deferred ButiEngine::ButiRendering::Resource_MaterialList_Dx12::GetMaterialDeferredValue()const
{
	return MaterialValue_Deferred();
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::SetMaterialIndex(const std::int32_t arg_index)
{
	materialVar.materialID = arg_index;
}

void ButiEngine::ButiRendering::Resource_MaterialList_Dx12::SetMaterialList(const List<Value_ptr<IResource_Material>>& arg_list_material)
{
	m_list_material = arg_list_material;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterial(const MaterialValue& arg_var, Value_weak_ptr<IResource_Texture> arg_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return CreateMaterial(arg_var, { arg_texture }, arg_vwp_graphicDevice);
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Material> ButiEngine::ButiRendering::CreateMaterial(const MaterialValue& arg_var, const List<Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	return ObjectFactory::Create<ButiRendering::Resource_Material_Dx12>(arg_var, arg_list_texture, arg_vwp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
}

