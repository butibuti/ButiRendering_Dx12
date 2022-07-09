#include "stdafx.h"
#include "ButiRendering_Dx12\Header\Resource_Material.h"



ButiEngine::ButiRendering::Resource_Material::Resource_Material(const MaterialValue& arg_var, Value_ptr<IResource_Shader> arg_vlp_shader,List<Value_ptr<IResource_Texture>> arg_list_vwp_texture)
:m_list_vlp_texture(arg_list_vwp_texture),materialVar(arg_var),m_vlp_shader(arg_vlp_shader){}

const std::string& ButiEngine::ButiRendering::Resource_Material::GetMaterialName()
{
	return materialName;
}

void ButiEngine::ButiRendering::Resource_Material::SetMaterialName(const std::string& arg_name)
{
	materialName = arg_name;
}


const std::string& ButiEngine::ButiRendering::Resource_Material::GetTagName() const
{
	return materialName;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Resource_Material::GetTexture(const std::int32_t arg_index)const
{
	return m_list_vlp_texture.At(arg_index);
}


void ButiEngine::ButiRendering::Resource_Material::SetTexture(const std::uint32_t arg_index, Value_ptr<IResource_Texture> arg_vwp_texture)
{
	if (arg_index >=m_list_vlp_texture.GetSize()) {
		for (std::int32_t i = 0; m_list_vlp_texture.GetSize() < arg_index + 1; i++) {
			m_list_vlp_texture.Add(Value_weak_ptr<IResource_Texture>());
		}
	}
	m_list_vlp_texture[arg_index] = arg_vwp_texture;
}

void ButiEngine::ButiRendering::Resource_Material::SetTexture(const List<Value_ptr<IResource_Texture>>& arg_list_vwp_textures)
{
	m_list_vlp_texture = arg_list_vwp_textures;
}

