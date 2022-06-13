#include "stdafx.h"
#include "ButiRendering_Dx12\Header\Resource_Material.h"



ButiEngine::ButiRendering::Resource_Material::Resource_Material(const MaterialValue& arg_var)
{
	materialVar = arg_var;
}

const std::string& ButiEngine::ButiRendering::Resource_Material::GetMaterialName()
{
	return materialName;
}

const std::string& ButiEngine::ButiRendering::Resource_Material::GetMaterialNameEng()
{
	return materialNameEng;
}

const std::string& ButiEngine::ButiRendering::Resource_Material::GetMaterialComment()
{
	return comment;
}

void ButiEngine::ButiRendering::Resource_Material::SetMaterialName(const std::string& arg_name)
{
	materialName = arg_name;
}

void ButiEngine::ButiRendering::Resource_Material::SetMaterialNameEng(const std::string& arg_name)
{
	materialNameEng = arg_name;
}

void ButiEngine::ButiRendering::Resource_Material::SetComment(const std::string& arg_comment)
{
	comment = arg_comment;
}

const std::string& ButiEngine::ButiRendering::Resource_Material::GetTagName() const
{
	return materialName;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Resource_Material::GetTexture(const std::int32_t arg_index)const
{
	return list_vwp_texture.At(arg_index);
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Resource_Material::GetSphereTexture()const
{
	return vwp_texture;
}

void ButiEngine::ButiRendering::Resource_Material::SetTexture(const std::uint32_t arg_index, Value_weak_ptr<IResource_Texture> arg_vwp_texture)
{

	if (arg_index >= list_vwp_texture.GetSize()) {
		for (std::int32_t i = 0; list_vwp_texture.GetSize() < arg_index + 1; i++) {
			list_vwp_texture.Add(Value_weak_ptr<IResource_Texture>());
		}
	}
	list_vwp_texture[arg_index] = arg_vwp_texture;
}

void ButiEngine::ButiRendering::Resource_Material::SetTexture(const List<Value_weak_ptr<IResource_Texture>>& arg_list_vwp_textures)
{
	list_vwp_texture = arg_list_vwp_textures;
}

void ButiEngine::ButiRendering::Resource_Material::SetSphereTexture(Value_weak_ptr<IResource_Texture> arg_vwp_texture)
{
	vwp_texture = arg_vwp_texture;
}
