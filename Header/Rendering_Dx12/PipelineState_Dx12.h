#ifndef PIPELINESTATE_DX12_H
#define PIPELINESTATE_DX12_H
#include<d3d12.h>
namespace ButiEngine {
namespace ButiRendering {

class RootSignature_Dx12 :public IRootSignature {
public:
	RootSignature_Dx12(Value_ptr<IResource_Shader> arg_vlp_shader, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	void Initialize()override;
	void Attach() override;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_rootSignature; }
	D3D12_ROOT_SIGNATURE_DESC& GetRootSignatureDesc() { return m_rootdesc; }
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
	D3D12_ROOT_SIGNATURE_DESC m_rootdesc;
	Value_weak_ptr<IResource_Shader> m_vwp_shader;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
};
class PipelineState_Dx12 :public IPipelineState {
public:
	PipelineState_Dx12(const DrawSettings& arg_drawSettings, Value_ptr<IRootSignature> arg_vlp_rootSignature, Value_weak_ptr<IResource_Shader> arg_vwp_shader, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	void Initialize()override; 
	void Attach() override;
	Value_ptr<IRootSignature> GetRootSignature() const { return m_vlp_rootSignature; }
	Value_ptr<IResource_Shader> GetShader()const override { return m_vwp_shader.lock(); }
private:
	Value_ptr<RootSignature_Dx12> m_vlp_rootSignature;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipeLineDesc= D3D12_GRAPHICS_PIPELINE_STATE_DESC();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
	D3D12_RASTERIZER_DESC m_rasterizerStateDesc= D3D12_RASTERIZER_DESC();
	Value_weak_ptr<IResource_Shader> m_vwp_shader;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	const DrawSettings& m_drawSettings;
};
}
}
#endif // !PIPELINESTATE_DX12_H
