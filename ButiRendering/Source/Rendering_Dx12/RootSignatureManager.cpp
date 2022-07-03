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

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IRootSignature> ButiEngine::ButiRendering::RootSignatureManager::GetRootSignature(const std::int32_t arg_srvCount, const std::int32_t arg_cBufferCount, const std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& arg_samplerHandles)
{
	std::string key = "SrvSmpCbvMat:" + std::to_string(arg_cBufferCount) + "srv:" + std::to_string(arg_srvCount) + "sampler:" + std::to_string(arg_samplerHandles.size());
	if (m_map_rootSignature.at(key)) {
		m_map_rootSignature.at(key);
	}
	auto output = ObjectFactory::Create<RootSignature_Dx12>(arg_srvCount,arg_cBufferCount,arg_samplerHandles,m_vwp_graphicDevice);
	m_map_rootSignature.emplace(key, output);
	return output;
}
