#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"


ButiEngine::ButiRendering::PipelineStateManager::PipelineStateManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
{
	vwp_graphicDevice = arg_vwp_graphicDevice;
}
void ButiEngine::ButiRendering::PipelineStateManager::Release()
{
	ClearPipelineState();
}

void ButiEngine::ButiRendering::PipelineStateManager::ClearPipelineState()
{
	map_pipeLineState.clear();
}


std::int32_t ButiEngine::ButiRendering::PipelineStateManager::GetPipelineStatePSOutputCount(const std::string& arg_pipeLineState)
{
	return map_pipeLineState.at(arg_pipeLineState)->GetPSOutputCount();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> ButiEngine::ButiRendering::PipelineStateManager::GetPipelineState(const std::string& arg_pipeLineState)
{
	return map_pipeLineState.at(arg_pipeLineState)->GetPielineState();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> ButiEngine::ButiRendering::PipelineStateManager::GetPipelineState(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, const D3D12_ROOT_SIGNATURE_DESC rootdesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, Value_ptr<IResource_Shader> arg_shader, const BlendMode arg_BlendMode, const TopologyType arg_topologyType,const bool arg_isDepth)
{
	auto rasterStr = std::to_string(arg_rasteriserDesc.CullMode) + std::to_string(arg_rasteriserDesc.FillMode) + std::to_string(arg_rasteriserDesc.AntialiasedLineEnable) + std::to_string(arg_rasteriserDesc.ConservativeRaster);
	auto rootStr = std::to_string(rootdesc.Flags) + std::to_string(rootdesc.NumParameters) + std::to_string(rootdesc.NumStaticSamplers);
	auto blend = std::to_string((std::int32_t)arg_BlendMode);
	std::string key = arg_shader->GetShaderName() + "/" + rasterStr + "/" + rootStr + "/" + blend;
	if (map_pipeLineState.count(key)) {
		return map_pipeLineState.at(key)->GetPielineState();
	}
	Microsoft::WRL::ComPtr<ID3DBlob> gemetryBlob;
	if (arg_shader->GetGeometryShader()) {
		gemetryBlob = arg_shader->GetGeometryShader()->GetThis<Resource_GeometryShader_Dx12>()->GetGeometryShaderBlob();
	}
	auto output = PipelineStateHelper::CreateDefault3D(rootSignature, RetDesc, arg_rasteriserDesc,
		arg_shader->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetInputLayoutVector(),
		arg_shader->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetVertexShaderBlob(),
		arg_shader->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetPixelShaderBlob(), gemetryBlob,
		arg_shader->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetFormats(),
		arg_BlendMode,arg_topologyType, vwp_graphicDevice.lock()->GetDevice(),arg_isDepth);
	map_pipeLineState.emplace(key, ObjectFactory::Create<PipelineState>(output) );
	return output;
}