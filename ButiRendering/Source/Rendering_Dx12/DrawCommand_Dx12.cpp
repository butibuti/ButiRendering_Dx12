#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DrawCommand_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/ShaderReflection.h"
#include"ButiRendering_Dx12/Header/Renderer.h"
ButiEngine::ButiRendering::DrawCommand_Dx12::DrawCommand_Dx12(const std::int32_t arg_offset, const std::int32_t arg_step, Value_weak_ptr<IDrawObject> arg_vwp_drawObject, Value_weak_ptr<IResource_Mesh> arg_vwp_mesh, Value_weak_ptr<IResource_Material> arg_vwp_material, Value_ptr<CBuffer<Matrices>> arg_vlp_matrixBuffer, std::vector<Value_ptr<ICBuffer>>& arg_ref_vec_exCBuffer, Value_weak_ptr<IRenderer> arg_vwp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
	:m_offset(arg_offset),m_step(arg_step),m_vwp_drawObject(arg_vwp_drawObject),m_vwp_mesh(arg_vwp_mesh),m_vwp_material(arg_vwp_material),m_vwp_graphicDevice(arg_vwp_graphicDevice),m_vlp_matrixBuffer(arg_vlp_matrixBuffer) {
	
	for (auto cBufferData : m_vwp_material.lock()->GetShader()->GetConstantBufferReflectionDatas()) {
		if (m_vlp_matrixBuffer->GetBufferName() == cBufferData.m_bufferName) {
			m_list_useExCBuffer.Add(m_vlp_matrixBuffer);
		}
		else if (arg_vwp_material.lock()->GetMaterialCBuffer()->GetBufferName() == cBufferData.m_bufferName) {
			m_list_useExCBuffer.Add(arg_vwp_material.lock()->GetMaterialCBuffer());
		}
		else if (arg_vwp_renderer.lock()->GetRendererCBuffer()->GetBufferName() == cBufferData.m_bufferName) {
			m_list_useExCBuffer.Add(arg_vwp_material.lock()->GetMaterialCBuffer());
		}
		for (auto cBuffer : arg_ref_vec_exCBuffer) {
			if (cBuffer->GetBufferName() == cBufferData.m_bufferName) {
				m_list_useExCBuffer.Add(cBuffer);
			}
		}
	}
}

void ButiEngine::ButiRendering::DrawCommand_Dx12::Execute()
{
	m_vwp_graphicDevice.lock()->SetMesh(m_vwp_mesh.lock(), m_vertexShaderType);
	m_vwp_graphicDevice.lock()->SetMaterial(m_vwp_material.lock());
	m_vlp_matrixBuffer->Attach(m_cBufferSlotOffset);
	
	for (std::int32_t registerSlot = 0, count = m_vwp_material.lock()->GetShader()->GetConstantBufferCount();registerSlot<count; registerSlot++) {
		m_vwp_graphicDevice.lock()->SetConstantBuffer(m_list_useExCBuffer[registerSlot], registerSlot+ m_cBufferSlotOffset);
	}

	m_vwp_graphicDevice.lock()->SetTopologyType(m_vwp_material.lock()->GetDrawSettings().topologyType);
	m_vwp_graphicDevice.lock()->GetCommandList().DrawIndexedInstanced(m_step, 1, m_offset, 0, 0);
}

void ButiEngine::ButiRendering::DrawCommand_Dx12::Execute_afterRendering()
{
	m_vwp_material.lock()->GetPipelineState()->GetRootSignature()->Attach();
	m_vwp_material.lock()->GetPipelineState()->Attach();
	Execute();
}

void ButiEngine::ButiRendering::DrawCommand_Dx12::Initialize()
{
	m_cBufferSlotOffset = m_vwp_material.lock()->GetShader()->GetSamplerReflectionDatas().GetSize() + m_vwp_material.lock()->GetShader()->GetTextureReflectionDatas().GetSize();
	m_vertexShaderType = m_vwp_material.lock()->GetShader()->GetVertexShader()->GetInputVertexType();
}
