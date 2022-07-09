#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/ShaderReflection.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"

ButiEngine::ButiRendering::PipelineStateManager::PipelineStateManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
:m_vwp_graphicDevice(arg_vwp_graphicDevice) {}
void ButiEngine::ButiRendering::PipelineStateManager::Release()
{
	ClearPipelineState();
}

void ButiEngine::ButiRendering::PipelineStateManager::ClearPipelineState()
{
	m_map_pipeLineState.clear();
}

std::int32_t ButiEngine::ButiRendering::PipelineStateManager::GetPipelineStatePSOutputCount(const std::string& arg_key)
{
	return m_map_pipeLineState.at(arg_key)->GetShader()->GetPixelShader()->GetOutputLayout().m_list_element.GetSize();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IPipelineState> ButiEngine::ButiRendering::PipelineStateManager::GetPipelineState(const std::string& arg_key)
{
	return m_map_pipeLineState.at(arg_key);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IPipelineState> ButiEngine::ButiRendering::PipelineStateManager::GetPipelineState(const DrawSettings& arg_drawSettings, Value_ptr<IRootSignature> arg_vlp_rootSignature, Value_weak_ptr<IResource_Shader> arg_vwp_shader)
{
	auto rasterStr = std::to_string(static_cast<std::int32_t>(arg_drawSettings.cullMode)) + std::to_string(static_cast<std::int32_t>(arg_drawSettings.isFill));
	auto rootStr = std::to_string(arg_vlp_rootSignature->GetThis<RootSignature_Dx12>()->GetRootSignatureDesc().Flags) + std::to_string(arg_vlp_rootSignature->GetThis<RootSignature_Dx12>()->GetRootSignatureDesc().NumParameters) + std::to_string(arg_vlp_rootSignature->GetThis<RootSignature_Dx12>()->GetRootSignatureDesc().NumStaticSamplers);
	auto blendStr = std::to_string(static_cast<std::int32_t>(arg_drawSettings.blendMode));
	std::string key = arg_vwp_shader.lock()->GetShaderName() + "/" + rasterStr + "/" + rootStr + "/" + blendStr;
	if (m_map_pipeLineState.count(key)) {
		return m_map_pipeLineState.at(key);
	}
	m_map_pipeLineState.emplace(key, ObjectFactory::Create<PipelineState_Dx12>(arg_drawSettings,arg_vlp_rootSignature,arg_vwp_shader,m_vwp_graphicDevice) );
	return m_map_pipeLineState.at(key);
}