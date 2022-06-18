#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Shader.h"


ButiEngine::ButiRendering::Resource_Shader::Resource_Shader(Value_ptr<IResource_VertexShader> arg_vlp_vertexShader, Value_ptr<IResource_PixelShader> arg_vlp_pixelShader, Value_ptr<IResource_GeometryShader> arg_vlp_geometryShader, const std::string& arg_shaderName)
{
	vlp_vertexShader = arg_vlp_vertexShader;
	vlp_pixelShader = arg_vlp_pixelShader;
	vlp_geometryShader = arg_vlp_geometryShader;
	shaderName = arg_shaderName;
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