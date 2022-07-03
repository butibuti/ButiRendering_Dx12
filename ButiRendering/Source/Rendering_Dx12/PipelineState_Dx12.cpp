#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"

ButiEngine::ButiRendering::RootSignature_Dx12::RootSignature_Dx12(const std::int32_t arg_srvCount, const std::int32_t arg_cBufferCount, const std::vector<D3D12_GPU_DESCRIPTOR_HANDLE>& arg_samplerHandles, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:m_textureRegion(arg_srvCount), m_vec_samplerBufferDescriptorHandle(arg_samplerHandles), m_vwp_graphicDevice(arg_vwp_graphicDevice), m_cBufferCount(arg_cBufferCount) {}
void ButiEngine::ButiRendering::RootSignature_Dx12::Initialize()
{
	m_rootSignature = RootSignatureHelper::CreateSrvSmpCbvMat(m_cBufferCount, m_textureRegion, m_vec_samplerBufferDescriptorHandle.size(), m_rootdesc, m_vwp_graphicDevice.lock()->GetDevice());
}
void ButiEngine::ButiRendering::RootSignature_Dx12::Attach()
{
	m_vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootSignature(m_rootSignature.Get());
}
ButiEngine::ButiRendering::PipelineState_Dx12::PipelineState_Dx12(const DrawSettings& arg_drawSettings, Value_ptr<IRootSignature> arg_vlp_rootSignature, Value_weak_ptr<IResource_Shader> arg_vwp_shader, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	: m_drawSettings(arg_drawSettings), m_vlp_rootSignature(arg_vlp_rootSignature->GetThis<RootSignature_Dx12>()), m_vwp_graphicDevice(arg_vwp_graphicDevice), m_vwp_shader(arg_vwp_shader){}

void ButiEngine::ButiRendering::PipelineState_Dx12::Initialize()
{
	m_rasterizerStateDesc = RasterizeDescHelper::GetDefaultRasterizerDesc();
	m_rasterizerStateDesc.CullMode = static_cast<D3D12_CULL_MODE>(m_drawSettings.cullMode);
	m_rasterizerStateDesc.FillMode = static_cast<D3D12_FILL_MODE>(m_drawSettings.isFill);
	//rasterizerStateDesc.MultisampleEnable = true;
	auto pipeLineTopology = m_drawSettings.topologyType;
	if (pipeLineTopology == TopologyType::triangleList) {
		pipeLineTopology = TopologyType::triangle;
	}
	else if (pipeLineTopology == TopologyType::pointList) {
		pipeLineTopology = TopologyType::point;
	}
	Microsoft::WRL::ComPtr<ID3DBlob> gemetryBlob;
	if (m_vwp_shader.lock()->GetGeometryShader()) {
		gemetryBlob = m_vwp_shader.lock()->GetGeometryShader()->GetThis<Resource_GeometryShader_Dx12>()->GetGeometryShaderBlob();
	}
	m_pipelineState = PipelineStateHelper::CreateDefault3D(m_vlp_rootSignature->GetRootSignature(),m_pipeLineDesc, m_rasterizerStateDesc,
		m_vwp_shader.lock()->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetInputLayoutVector(),
		m_vwp_shader.lock()->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetVertexShaderBlob(),
		m_vwp_shader.lock()->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetPixelShaderBlob(), gemetryBlob,
		m_vwp_shader.lock()->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetFormats(),
		m_drawSettings.blendMode, pipeLineTopology, m_vwp_graphicDevice.lock()->GetDevice(), m_drawSettings.isDepth);
}

void ButiEngine::ButiRendering::PipelineState_Dx12::Attach()
{
	m_vwp_graphicDevice.lock()->SetPipeLine(m_pipelineState);
}
