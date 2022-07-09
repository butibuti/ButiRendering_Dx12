#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/RootSignatureManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"

ButiEngine::ButiRendering::RootSignatureManager::RootSignatureManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
:m_vwp_graphicDevice(arg_vwp_graphicDevice) {}

void ButiEngine::ButiRendering::RootSignatureManager::Release()
{
	m_map_rootSignature.clear();
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRootSignature> ButiEngine::ButiRendering::RootSignatureManager::GetRootSignature(const std::string& arg_key)
{
	return m_map_rootSignature.at(arg_key);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRootSignature> ButiEngine::ButiRendering::RootSignatureManager::GetRootSignature(Value_ptr<IResource_Shader> arg_vlp_shader)
{
	std::string key = "SrvSmpCbvMat:" + std::to_string(arg_vlp_shader->GetConstantBufferReflectionDatas().GetSize()) + "srv:" + std::to_string(arg_vlp_shader->GetTextureReflectionDatas().GetSize()) + "sampler:" + std::to_string(arg_vlp_shader->GetSamplerReflectionDatas().GetSize());
	if (m_map_rootSignature.count(key)) {
		m_map_rootSignature.at(key);
	}
	auto output = ObjectFactory::Create<RootSignature_Dx12>(arg_vlp_shader,m_vwp_graphicDevice);
	m_map_rootSignature.emplace(key, output);
	return output;
}
