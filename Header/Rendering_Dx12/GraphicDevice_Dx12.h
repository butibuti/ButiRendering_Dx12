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
class RootSignatureManager;
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

	GraphicDevice_Dx12();
	void Initialize()override;
	void Release()override;
	void SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)override;
	void ClearDepthStancil(const float arg_depth = 1.0f) override;
	BUTIRENDERING_API ID3D12Device& GetDevice();
	BUTIRENDERING_API HRESULT CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState,
		const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidResource, void** ppvResource);
	BUTIRENDERING_API ID3D12CommandQueue& GetCommandQueue();
	BUTIRENDERING_API ID3D12CommandAllocator& GetCommandAllocator();
	BUTIRENDERING_API ID3D12CommandAllocator& GetCommandAllocator(const std::uint32_t arg_index);
	BUTIRENDERING_API ID3D12CommandAllocator& GetBundleCommandAllocator();
	BUTIRENDERING_API ID3D12GraphicsCommandList& GetCommandList();
	BUTIRENDERING_API ID3D12GraphicsCommandList& GetUploadCommandList();
	BUTIRENDERING_API virtual ID3D12GraphicsCommandList& GetClearCommandList();

	BUTIRENDERING_API void SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	BUTIRENDERING_API Value_weak_ptr<DescriptorHeapManager> GetDescriptorHeapManager();

	PipelineStateManager& GetPipelineStateManager();
	RootSignatureManager& GetRootSignatureManager();
	BUTIRENDERING_API void AddUploadResource(GPUResource* rarg_resource);

	BUTIRENDERING_API void AddOutputResource(GPUResource* rarg_resource, const FileFormat arg_format, const std::string& arg_fileName);


	BUTIRENDERING_API void UploadResourceRelease()override;
	BUTIRENDERING_API void UploadResourceBufferMerge()override;
	BUTIRENDERING_API void UnSetCommandList();

	BUTIRENDERING_API ID3D12Fence& GetFence();
	BUTIRENDERING_API virtual IDXGISwapChain& GetSwapChain();



	BUTIRENDERING_API virtual void WaitGPU();

	BUTIRENDERING_API void Present() override;

	BUTIRENDERING_API void DrawEnd();
	BUTIRENDERING_API void StartGUICommand();
	BUTIRENDERING_API void EndGUICommand();

	BUTIRENDERING_API void Reset() override;
	BUTIRENDERING_API void ClearWindow() override;


	BUTIRENDERING_API void SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList, const std::int32_t index = 0);
	BUTIRENDERING_API void SetDefaultRenderTarget()override;
	BUTIRENDERING_API void SetDefaultDepthStencil();
	BUTIRENDERING_API void CommandList_SetScissorRect();

	BUTIRENDERING_API void CommandList_SetRenderTargetView();
	BUTIRENDERING_API void PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle,Value_ptr<IRenderTarget> arg_vlp_renderTarget);
	BUTIRENDERING_API void InsertCommandList();
	BUTIRENDERING_API void ResourceUpload()override;

	BUTIRENDERING_API void DrawStart()override;
	BUTIRENDERING_API void PipelineClear()override;
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;

	BUTIRENDERING_API ID3D12Resource& GetTextureUploadHeap();

	BUTIRENDERING_API D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;
	BUTIRENDERING_API D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;
	BUTIRENDERING_API const D3D12_RECT& GetScissorRect()const;
	BUTIRENDERING_API virtual std::uint32_t GetFrameCount() const;
	BUTIRENDERING_API virtual std::uint32_t GetFrameIndex()const;
	BUTIRENDERING_API void SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE* arg_dsv,Value_ptr<IDepthStencil> arg_vlp_depthStencil);
	BUTIRENDERING_API D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil();
	BUTIRENDERING_API void DisSetDepthStencil();
	BUTIRENDERING_API void ResetPipeLine()override;
protected:
	class Impl;
	std::unique_ptr<Impl> m_uqp_impl;
};

BUTIRENDERING_API Value_ptr< GraphicDevice_Dx12> CreateGraphicDeviceDx12(const bool arg_isWindowApp);

}
}
