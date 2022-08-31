#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineState_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"

ButiEngine::ButiRendering::RootSignature_Dx12::RootSignature_Dx12(Value_ptr<IResource_Shader> arg_vlp_shader,  Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:m_vwp_graphicDevice(arg_vwp_graphicDevice),m_vwp_shader(arg_vlp_shader){}
void ButiEngine::ButiRendering::RootSignature_Dx12::Initialize()
{
	m_rootSignature = RootSignatureHelper::CreateSrvSmpCbvMat(m_vwp_shader.lock()->GetConstantBufferReflectionDatas(), m_vwp_shader.lock()->GetTextureReflectionDatas().GetSize(), m_vwp_shader.lock()->GetSamplerReflectionDatas() .GetSize(), m_rootdesc, m_vwp_graphicDevice.lock()->GetDevice());
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
	List<Format> list_format;
	for (auto outputElem : m_vwp_shader.lock()->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetOutputLayout().m_list_element) {
		if (m_vwp_shader.lock()->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetOutputLayout().m_list_element.GetSize() == 1
			&& outputElem.format == Format::R32G32B32A32_FLOAT) {
			list_format.Add(Format::R8G8B8A8_UNORM);
		}
		else if (outputElem.format == Format::R32G32B32A32_FLOAT&&
			(outputElem.semanticIndex== 0 || outputElem.semanticIndex == 1)) {
			list_format.Add(Format::R8G8B8A8_UNORM);
		}
		else {
			list_format.Add(outputElem.format);
		}
	}
	m_pipelineState = PipelineStateHelper::CreateDefault3D(m_vlp_rootSignature->GetRootSignature(),m_pipeLineDesc, m_rasterizerStateDesc,
		m_vwp_shader.lock()->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetInputLayoutList(),
		m_vwp_shader.lock()->GetVertexShader()->GetThis<Resource_VertexShader_Dx12>()->GetVertexShaderBlob(),
		m_vwp_shader.lock()->GetPixelShader()->GetThis<Resource_PixelShader_Dx12>()->GetPixelShaderBlob(), gemetryBlob,
		list_format,
		m_drawSettings.blendMode, pipeLineTopology, m_vwp_graphicDevice.lock()->GetDevice(), m_drawSettings.isDepth);
}

void ButiEngine::ButiRendering::PipelineState_Dx12::Attach()
{
	m_vwp_graphicDevice.lock()->SetPipeLine(m_pipelineState);
}
