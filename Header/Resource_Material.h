#pragma once
#include "stdafx.h"
#include"ResourceInterface.h"
namespace ButiEngine {

namespace ButiRendering {
struct Resource_Material :public IResource_Material
{
	BUTIRENDERING_API Resource_Material(const MaterialValue& arg_var);
	void Initialize()override {};
	void PreInitialize()override {};
	BUTIRENDERING_API const std::string& GetMaterialName();
	BUTIRENDERING_API const std::string& GetMaterialNameEng();
	BUTIRENDERING_API const std::string& GetMaterialComment();
	BUTIRENDERING_API void SetMaterialName(const std::string& arg_name);
	BUTIRENDERING_API void SetMaterialNameEng(const std::string& arg_name);
	BUTIRENDERING_API void SetComment(const std::string& arg_comment);
	BUTIRENDERING_API const std::string& GetTagName()const override;
	BUTIRENDERING_API Value_weak_ptr<IResource_Texture>GetTexture(const std::int32_t index = 0)const;
	BUTIRENDERING_API Value_weak_ptr<IResource_Texture>GetSphereTexture()const;
	BUTIRENDERING_API void SetTexture(const std::uint32_t arg_index, Value_weak_ptr<IResource_Texture>  arg_vwp_texture)override;
	BUTIRENDERING_API void SetTexture(const List<Value_weak_ptr<IResource_Texture>>& arg_list_vwp_textures)override;
	BUTIRENDERING_API void SetSphereTexture(Value_weak_ptr<IResource_Texture>  arg_vwp_texture)override;
	const List<Value_weak_ptr<IResource_Texture>>& GetTextures()const override {
		return list_vwp_texture;
	}
	Value_ptr<IResource_Shader> GetShader()const { return m_vlp_shader; }
	void SetShader(Value_ptr<IResource_Shader> arg_vlp_shader) { m_vlp_shader = arg_vlp_shader; }
	const DrawSettings& GetDrawSettings()const override{ return m_drawSetting; }
	DrawSettings& GetDrawSettings()override{ return m_drawSetting; }
	Value_ptr<IPiplineState> GetPipelineState()const override { return m_vlp_pipelineState; }
protected:
	MaterialValue materialVar;
	std::string materialName;
	std::string materialNameEng;
	std::string comment;
	DrawSettings m_drawSetting;
	Value_ptr<IResource_Shader> m_vlp_shader;
	Value_ptr<IPiplineState> m_vlp_pipelineState;
	List<Value_weak_ptr<IResource_Texture>> list_vwp_texture;
};
}
}