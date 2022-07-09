#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Shader.h"
#include<algorithm>

ButiEngine::ButiRendering::Resource_Shader::Resource_Shader(Value_ptr<IResource_VertexShader> arg_vlp_vertexShader, Value_ptr<IResource_PixelShader> arg_vlp_pixelShader, Value_ptr<IResource_GeometryShader> arg_vlp_geometryShader, const std::string& arg_shaderName)
	:vlp_vertexShader(arg_vlp_vertexShader), vlp_pixelShader(arg_vlp_pixelShader), vlp_geometryShader(arg_vlp_geometryShader), shaderName(arg_shaderName)
{}

void ButiEngine::ButiRendering::Resource_Shader::Initialize()
{
	m_list_cBufferReflection = vlp_vertexShader->GetConstantBufferReflectionDatas();
	m_list_cBufferReflection.Add_noDuplicate(vlp_pixelShader->GetConstantBufferReflectionDatas());
	if (vlp_geometryShader) {
		m_list_cBufferReflection.Add_noDuplicate(vlp_geometryShader->GetConstantBufferReflectionDatas());
	}
	std::sort(m_list_cBufferReflection.begin(), m_list_cBufferReflection.end(),
		[](const ConstantBufferReflection& arg_x,const ConstantBufferReflection& arg_y)->bool {return arg_x.m_slot < arg_y.m_slot; });
	/*for (std::int32_t currentSlot = 0, lastSlot = m_list_cBufferReflection.GetLast().m_slot; currentSlot != lastSlot; currentSlot++) {
		if (m_list_cBufferReflection.At(currentSlot).m_slot != currentSlot) {
			m_list_cBufferReflection.Insert(currentSlot, { currentSlot,0,"None" });
		}
	}*/
	m_list_samplerReflection = vlp_vertexShader->GetSamplerReflectionDatas();
	m_list_samplerReflection.Add_noDuplicate(vlp_pixelShader->GetSamplerReflectionDatas());
	if (vlp_geometryShader) {
		m_list_samplerReflection.Add_noDuplicate(vlp_geometryShader->GetSamplerReflectionDatas());
	}
	m_list_textureReflection = vlp_vertexShader->GetTextureReflectionDatas();
	m_list_textureReflection.Add_noDuplicate(vlp_pixelShader->GetTextureReflectionDatas());
	if (vlp_geometryShader) {
		m_list_textureReflection.Add_noDuplicate(vlp_geometryShader->GetTextureReflectionDatas());
	}
}

void ButiEngine::ButiRendering::Resource_Shader::Attach() const
{
	vlp_vertexShader->Attach();
	vlp_pixelShader->Attach();
	if (vlp_geometryShader) {
		vlp_geometryShader->Attach();
	}
}

bool ButiEngine::ButiRendering::Resource_Shader::GetIsContainGeometryShader() const
{
	if (vlp_geometryShader) {
		return true;
	}
	return false;
}

std::string ButiEngine::ButiRendering::Resource_Shader::GetShaderName()const
{
	return shaderName;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::Resource_Shader::GetVertexShader()
{
	return vlp_vertexShader;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::Resource_Shader::GetPixelShader()
{
	return vlp_pixelShader;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::Resource_Shader::GetGeometryShader()
{
	return vlp_geometryShader;
}