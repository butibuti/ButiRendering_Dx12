#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Resource_Shader.h"


ButiEngine::ButiRendering::Resource_Shader::Resource_Shader(Value_weak_ptr<IResource_VertexShader> arg_vwp_vertexShader, Value_weak_ptr<IResource_PixelShader> arg_vwp_pixelShader, Value_weak_ptr<IResource_GeometryShader> arg_vwp_geometryShader, const std::string& arg_shaderName)
{
	vwp_vertexShader = arg_vwp_vertexShader;
	vwp_pixelShader = arg_vwp_pixelShader;
	vwp_geometryShader = arg_vwp_geometryShader;
	shaderName = arg_shaderName;
}

void ButiEngine::ButiRendering::Resource_Shader::Attach() const
{
	vwp_vertexShader.lock()->Attach();
	vwp_pixelShader.lock()->Attach();
	if (vwp_geometryShader.lock()) {
		vwp_geometryShader.lock()->Attach();
	}
}

bool ButiEngine::ButiRendering::Resource_Shader::GetIsContainGeometryShader() const
{
	if (vwp_geometryShader.lock()) {
		return true;
	}
	return false;
}

void ButiEngine::ButiRendering::Resource_Shader::ShowGUI()
{
	std::string windowName = shaderName;

	GUIWindowReaction output;
	bool isOpen = true;
	GUI::Begin(windowName, &isOpen);
	if (!isOpen) {
		output.isClosed = true;
	}
	GUI::BulletText("VertexShader:" + vwp_vertexShader.lock()->GetName());
	GUI::BulletText("PixelShader:" + vwp_pixelShader.lock()->GetName());
	if (vwp_geometryShader.lock()) {
		GUI::BulletText("GeometryShader:" + vwp_geometryShader.lock()->GetName());
	}

	if (output.isClosed) {
		UnRegistEditorGUI();
	}

	GUI::End();

}

std::string ButiEngine::ButiRendering::Resource_Shader::GetShaderName()const
{
	return shaderName;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_VertexShader> ButiEngine::ButiRendering::Resource_Shader::GetVertexShader()
{
	return vwp_vertexShader;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_PixelShader> ButiEngine::ButiRendering::Resource_Shader::GetPixelShader()
{
	return vwp_pixelShader;
}

ButiEngine::Value_weak_ptr<ButiEngine::ButiRendering::IResource_GeometryShader> ButiEngine::ButiRendering::Resource_Shader::GetGeometryShader()
{
	return vwp_geometryShader;
}