#pragma once
#include"../GraphicDevice.h"
#include <d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<mutex>
namespace ButiEngine {

namespace ButiRendering {
class DescriptorHeapManager;
class PipelineStateManager;
enum FileFormat
{
	None, PNG, JPEG, BMP
};
class GraphicDevice_Dx12 :public GraphicDevice
{
	struct ImageFileOutputInfo {
		GPUResource* p_outputResource;
		std::string outputName;
		FileFormat format;
	};
public:

	GraphicDevice_Dx12(Value_weak_ptr<IApplication> arg_vwp_application);
	void Initialize()override;
	void Release()override;
	void SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)override;
	void ClearDepthStancil(const float arg_depth = 1.0f) override;
	ID3D12Device& GetDevice();
	HRESULT CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState,
		const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidResource, void** ppvResource);

	ID3D12CommandQueue& GetCommandQueue();

	ID3D12CommandAllocator& GetCommandAllocator();

	ID3D12CommandAllocator& GetCommandAllocator(const std::uint32_t arg_index);

	ID3D12CommandAllocator& GetBundleCommandAllocator();

	ID3D12GraphicsCommandList& GetCommandList();

	ID3D12GraphicsCommandList& GetUploadCommandList();
	virtual ID3D12GraphicsCommandList& GetClearCommandList();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbvMat(const std::uint32_t materialCount, const std::uint32_t srvCount, const std::uint32_t samplerCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc);

	void SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	std::pair< Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> GetRootSignature(const std::wstring& Key);

	Value_weak_ptr<DescriptorHeapManager> GetDescriptorHeapManager();

	PipelineStateManager& GetPipelineStateManager();

	void SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc);

	void AddUploadResource(GPUResource* rarg_resource);

	void AddOutputResource(GPUResource* rarg_resource, const FileFormat arg_format, const std::string& arg_fileName);


	void UploadResourceRelease()override;
	void UploadResourceBufferMerge()override;

	void UnSetCommandList();

	ID3D12Fence& GetFence();
	virtual IDXGISwapChain& GetSwapChain();



	virtual void WaitGPU();

	void Present() override;

	void DrawEnd();
	void StartGUICommand();
	void EndGUICommand();

	void Reset() override;
	void ClearWindow() override;


	void SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList, const std::int32_t index = 0);
	void SetDefaultRenderTarget()override;
	void SetDefaultDepthStencil();
	void CommandList_SetScissorRect();

	void CommandList_SetRenderTargetView();

	void PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle);

	void InsertCommandList();

	void ResourceUpload()override;

	void DrawStart()override;
	void PipelineClear()override;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;

	ID3D12Resource& GetTextureUploadHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;

	const D3D12_RECT& GetScissorRect()const;

	virtual std::uint32_t GetFrameCount() const;

	virtual std::uint32_t GetFrameIndex()const;

	void SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE* arg_dsv);
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil();
	void DisSetDepthStencil();
	void ResetPipeLine()override;
protected:
	class Impl;
	std::unique_ptr<Impl> m_uqp_impl;
};

Value_ptr< GraphicDevice_Dx12> CreateGraphicDeviceDx12(Value_weak_ptr<IApplication> arg_vwp_application,const bool arg_isWindowApp);

}
}
