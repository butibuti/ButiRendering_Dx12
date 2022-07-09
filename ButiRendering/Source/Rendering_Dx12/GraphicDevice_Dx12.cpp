#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/RootSignatureManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include "ButiRendering_Dx12\Header\Rendering_Dx12\GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12\Header\Rendering_Dx12\Resource_Texture_Dx12_DepthStencil.h"
#include<algorithm>
namespace ButiEngine {
namespace ButiRendering {

constexpr std::uint32_t FrameCount =3;

class GraphicDevice_Dx12::Impl {
public:
	D3D12_CPU_DESCRIPTOR_HANDLE* currentDSV = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE defaultDSVHandle;
	Microsoft::WRL::ComPtr<ID3D12Device> cmp_device;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmp_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmp_commandAllocator[FrameCount];
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmp_bundleCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmp_presentCommandList;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmp_uploadCommandList;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmp_drawCommandList;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmp_guiCommandList;
	Microsoft::WRL::ComPtr<ID3D12Fence>  cmp_fence;
	Microsoft::WRL::ComPtr<ID3D12Resource> cmp_renderTargets[FrameCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> cmp_resourceUploadHeap;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>  cmp_clearRootSignature;
	Microsoft::WRL::ComPtr<IDXGIFactory4> cmp_factory4;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  cmp_renderTargetDescriptorHeap;

	List<GPUResource*> list_uploadResources;
	List<GPUResource*> list_uploadResourcesBuffer;
	List<GPUResource*> list_befUploadResources;
	Value_ptr<DescriptorHeapManager> vlp_DescriptorManager;

	Value_ptr<PipelineStateManager> vlp_pipelineStateManager;
	Value_ptr<RootSignatureManager> vlp_rootSignatureManager;

	List<D3D12_CPU_DESCRIPTOR_HANDLE> list_renderTargetHandles;
	List<Value_ptr<IRenderTarget>>list_vlp_usedRenderTargets;
	List<Value_ptr<IDepthStencil>>list_vlp_usedDepthStencils;

	ID3D12GraphicsCommandList* p_currentCommandList = nullptr;
	ID3D12PipelineState* currentPipelineState = nullptr;

	List< ID3D12CommandList*> vec_drawCommandLists;
	std::uint32_t depthDescriptorSize, renderTargetDescriptorSize;
	D3D12_CPU_DESCRIPTOR_HANDLE ary_rtvHandle[FrameCount];
	D3D12_RECT scissorRect;
	std::uint32_t frameIndex = 0;

	HANDLE  fenceEvent;
	UINT64  fenceValue;

	std::vector<ImageFileOutputInfo > vec_outputInfo;
	std::mutex mtx_uploadResource, mtx_createCommitedResourece;
	D3D_FEATURE_LEVEL level;
};

class GraphicDevice_Dx12_WithWindow :public GraphicDevice_Dx12{
public:
	GraphicDevice_Dx12_WithWindow():GraphicDevice_Dx12(){}

	std::uint32_t GetFrameIndex()const override {
		return m_uqp_impl->frameIndex;
	}
	void Present()override;
	void WaitGPU()override;
	void Release()override;
	ID3D12GraphicsCommandList& GetClearCommandList()override;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> clearCommandList;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
	IDXGISwapChain& GetSwapChain()override;
	void ClearWindow()override;
	void SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)override;
	std::uint32_t GetFrameCount()const override;
};

void GraphicDevice_Dx12_WithWindow::Present()
{

	m_uqp_impl->currentPipelineState = nullptr;
	auto hr = m_uqp_impl->cmp_presentCommandList->Reset(m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get(), nullptr);
	//プレゼント用のバリアを張る
	auto desc = ResourceBarrierHelper::GetResourceBarrierTransition(m_uqp_impl->cmp_renderTargets[GetFrameIndex()].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_uqp_impl->cmp_presentCommandList->ResourceBarrier(1,
		&desc);
	SetCommandList(m_uqp_impl->cmp_presentCommandList.Get());
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		itr->p_outputResource->CopyForOutput();
	}
	hr = m_uqp_impl->cmp_presentCommandList->Close();
	InsertCommandList();

	if (m_uqp_impl->vec_drawCommandLists.GetSize()) {
		m_uqp_impl->cmp_commandQueue->ExecuteCommandLists(static_cast<std::uint32_t>(m_uqp_impl->vec_drawCommandLists.GetSize()), &(m_uqp_impl->vec_drawCommandLists[0]));

		const UINT64 fenceV = m_uqp_impl->fenceValue;
		auto hr = m_uqp_impl->cmp_commandQueue->Signal(m_uqp_impl->cmp_fence.Get(), fenceV);

		if (hr != S_OK) {
			auto reason = m_uqp_impl->cmp_device->GetDeviceRemovedReason();
			assert(0);
		}
	}

	hr = swapChain3->Present(0, 0);
	WaitGPU();
	m_uqp_impl->fenceValue++;
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		auto res = itr->p_outputResource;
		res->OutputToMemory();
	}
	m_uqp_impl->vec_outputInfo.clear();
	m_uqp_impl->vec_drawCommandLists.Clear();

	for (auto usedRenderTarget : m_uqp_impl->list_vlp_usedRenderTargets) {
		usedRenderTarget->SetIsCleared(false);
	}
	m_uqp_impl->list_vlp_usedRenderTargets.Clear();
	for (auto usedDepthStencil : m_uqp_impl->list_vlp_usedDepthStencils) {
		usedDepthStencil->SetIsCleared(false);
	}
	m_uqp_impl->list_vlp_usedDepthStencils.Clear();
}
void GraphicDevice_Dx12_WithWindow::WaitGPU()
{
	GraphicDevice_Dx12::WaitGPU();

	m_uqp_impl->frameIndex = swapChain3->GetCurrentBackBufferIndex();
}
void GraphicDevice_Dx12_WithWindow::Release()
{
	clearCommandList = nullptr;
	swapChain3 = nullptr;
	GraphicDevice_Dx12::Release();
}
ID3D12GraphicsCommandList& GraphicDevice_Dx12_WithWindow::GetClearCommandList()
{
	return *clearCommandList.Get();
}
IDXGISwapChain& ButiRendering::GraphicDevice_Dx12_WithWindow::GetSwapChain()
{
	return *swapChain3.Get();
}
void ButiRendering::GraphicDevice_Dx12_WithWindow::ClearWindow()
{
	clearCommandList->Reset(m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get(), nullptr);

	clearCommandList->SetGraphicsRootSignature(m_uqp_impl->cmp_clearRootSignature.Get());


	D3D12_RESOURCE_BARRIER desc = ResourceBarrierHelper::GetResourceBarrierTransition(m_uqp_impl->cmp_renderTargets[GetFrameIndex()].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	clearCommandList->ResourceBarrier(1,
		&desc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_uqp_impl->cmp_renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += GetFrameIndex() * m_uqp_impl->renderTargetDescriptorSize;

	clearCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &m_uqp_impl->defaultDSVHandle);


	clearCommandList->ClearRenderTargetView(rtvHandle, &m_clearColor.x, 0, nullptr);
	clearCommandList->ClearDepthStencilView(m_uqp_impl->defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	CommandListHelper::Close(clearCommandList);
	SetCommandList(clearCommandList.Get());
	InsertCommandList();
}
void ButiRendering::GraphicDevice_Dx12_WithWindow::SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = GetFrameCount();
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	DXGI_SWAP_CHAIN_DESC swapChaindesc = {};
	swapChaindesc.BufferCount = GetFrameCount();
	swapChaindesc.BufferDesc.Width = arg_width;
	swapChaindesc.BufferDesc.Height = arg_height;
	swapChaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChaindesc.OutputWindow = *reinterpret_cast<HWND*>(&arg_handle);
	swapChaindesc.Windowed = true;
	swapChaindesc.SampleDesc.Count = 1;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	//Create SwapChain.
	auto hr = m_uqp_impl->cmp_factory4->CreateSwapChain(m_uqp_impl->cmp_commandQueue.Get(), &swapChaindesc, swapChain.GetAddressOf());
	assert(hr == S_OK);

	// To IDXGISwapChain3.
	hr = swapChain->QueryInterface(IID_PPV_ARGS(swapChain3.GetAddressOf()));
	assert(hr == S_OK);

	//Get FrameBufferIndex.
	m_uqp_impl->frameIndex = swapChain3->GetCurrentBackBufferIndex();

	hr = m_uqp_impl->cmp_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_uqp_impl->cmp_renderTargetDescriptorHeap.GetAddressOf()));
	assert(hr == S_OK);
	//for clear
	clearCommandList = CommandListHelper::CreateSimple(*m_uqp_impl->cmp_device.Get(), *m_uqp_impl->cmp_commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(clearCommandList);

	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	handle = m_uqp_impl->cmp_renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewdesc = {};
	renderTargetViewdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewdesc.Texture2D.MipSlice = 0;
	renderTargetViewdesc.Texture2D.PlaneSlice = 0;

	for (std::uint32_t i = 0; i < GetFrameCount(); i++)
	{
		// Get BackBuffer
		hr = swapChain3->GetBuffer(i, IID_PPV_ARGS(m_uqp_impl->cmp_renderTargets[i].GetAddressOf()));
		assert(hr == S_OK);
		// Create RTV
		m_uqp_impl->cmp_device->CreateRenderTargetView(m_uqp_impl->cmp_renderTargets[i].Get(), &renderTargetViewdesc, handle);
		handle.ptr += m_uqp_impl->renderTargetDescriptorSize;
	}

	auto depthData = make_value<ImageFileIO::TextureResourceData>();
	depthData ->pixSize = sizeof(float);
	depthData ->width = arg_width;
	depthData ->height = arg_height;
	auto ds= ObjectFactory::Create<Resource_Texture_Dx12_DepthStencil>(depthData, GetThis<GraphicDevice_Dx12>());
	m_defaultDepthStencil = ds;
	m_uqp_impl->defaultDSVHandle = ds->GetHandle();
	m_uqp_impl->defaultDSVHandle = ds->GetHandle();
	m_uqp_impl->scissorRect.left = 0;
	m_uqp_impl->scissorRect.right = arg_width;
	m_uqp_impl->scissorRect.top = 0;
	m_uqp_impl->scissorRect.bottom = arg_height;
	
}
std::uint32_t GraphicDevice_Dx12_WithWindow::GetFrameCount() const
{
	return FrameCount;
}
}
}
ButiEngine::ButiRendering::GraphicDevice_Dx12::GraphicDevice_Dx12()
{
	m_uqp_impl = std::make_unique<Impl>();
}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::Initialize()
{
	HRESULT hr;

	UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after cmp_device creation will invalidate the active cmp_device.
	{
		Microsoft::WRL:: ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	hr = CreateDXGIFactory1(IID_PPV_ARGS(m_uqp_impl-> cmp_factory4.GetAddressOf()));

	IDXGIAdapter1* p_adapter = nullptr;
	{

		std::vector<IDXGIAdapter1*> vec_p_adapters;

		for (std::int32_t i = 0; m_uqp_impl->cmp_factory4->EnumAdapters1(i, &p_adapter) != DXGI_ERROR_NOT_FOUND; i++) {
			vec_p_adapters.push_back(p_adapter);
			DXGI_ADAPTER_DESC1 adapterDesc;
			p_adapter->GetDesc1(&adapterDesc);
			auto name = std::wstring(adapterDesc.Description);
			std::wcout <<name<<"|" << L"DedicatedSystemMemory:" << adapterDesc.DedicatedSystemMemory 
				<< L" DedicatedVideoMemory:" << adapterDesc.DedicatedVideoMemory
				<< L" SharedMemory:" << adapterDesc.SharedSystemMemory 
			<< L" AdapterType:" << adapterDesc.Flags << std::endl;
		}

		std::sort(vec_p_adapters.begin(), vec_p_adapters.end(), [](IDXGIAdapter1* x, IDXGIAdapter1* y)->bool {
			DXGI_ADAPTER_DESC1 adapterDesc_x, adapterDesc_y;
			x->GetDesc1(&adapterDesc_x);
			y->GetDesc1(&adapterDesc_y);
			return adapterDesc_x.DedicatedVideoMemory < adapterDesc_y.DedicatedVideoMemory;
			});

		for (std::int32_t i = vec_p_adapters.size() - 1; i >= 0; i--) {
			DXGI_ADAPTER_DESC1 adapterDesc;
			vec_p_adapters.at(i)->GetDesc1(&adapterDesc);

			auto name = std::wstring(adapterDesc.Description);
			if (!StringHelper::Contains(name, L"Microsoft") && !StringHelper::Contains(name, L"Intel")) {
				p_adapter = vec_p_adapters.at(i);
				std::wcout<<L"Use:" << name << std::endl;
				break;
			}
		}

		if (!p_adapter) {
			p_adapter = vec_p_adapters.at(0);
		}
	}

	
	{

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		for (std::int32_t i = 0; i < sizeof(levels) / sizeof(D3D_FEATURE_LEVEL); i++) {

			hr = D3D12CreateDevice(p_adapter, levels[i], IID_PPV_ARGS(m_uqp_impl->cmp_device.GetAddressOf()));
			if (hr == S_OK) {
				m_uqp_impl-> level = levels[i];
				break;
			}
		}
	}

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = m_uqp_impl->cmp_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(m_uqp_impl->cmp_commandQueue.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = GetFrameCount();
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;







	m_uqp_impl->renderTargetDescriptorSize = m_uqp_impl->cmp_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//Get DescriptorSize of DSV
	m_uqp_impl->depthDescriptorSize = m_uqp_impl->cmp_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);



	for (std::uint32_t i = 0; i < GetFrameCount(); i++)
	{
		HRESULT hr;

		hr = m_uqp_impl->cmp_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_uqp_impl->cmp_commandAllocator[i].GetAddressOf()));
		assert(hr == S_OK);

	}
	hr = m_uqp_impl->cmp_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(m_uqp_impl->cmp_bundleCommandAllocator.GetAddressOf()));
	assert(hr == S_OK);
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pParameters = nullptr;


	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	m_uqp_impl->cmp_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_uqp_impl->cmp_clearRootSignature));


	//for presentBarrier
	m_uqp_impl->cmp_presentCommandList = CommandListHelper::CreateSimple(*m_uqp_impl->cmp_device.Get(), *m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());
	CommandListHelper::Close(m_uqp_impl->cmp_presentCommandList);
	//for upload 
	m_uqp_impl->cmp_uploadCommandList=CommandListHelper::CreateSimple(*m_uqp_impl->cmp_device.Get(), *m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());
	CommandListHelper::Close(m_uqp_impl->cmp_uploadCommandList);
	//for draw
	m_uqp_impl->cmp_drawCommandList= CommandListHelper::CreateSimple(*m_uqp_impl->cmp_device.Get(), *m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());
	CommandListHelper::Close(m_uqp_impl->cmp_drawCommandList);

	//for gui
	m_uqp_impl->cmp_guiCommandList=CommandListHelper::CreateSimple(*m_uqp_impl->cmp_device.Get(), *m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());
	CommandListHelper::Close(m_uqp_impl->cmp_guiCommandList);

	// フェンスを生成
	hr = m_uqp_impl->cmp_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_uqp_impl->cmp_fence.GetAddressOf()));
	if (hr!=S_OK)
	{
		throw ButiException(L"フェンスの生成失敗");
	}

	// フェンスイベントを生成
	m_uqp_impl->fenceValue = 1;
	m_uqp_impl->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_uqp_impl->fenceEvent == nullptr)
	{
		throw ButiException(L"フェンスイベントの生成失敗");
	}

	m_uqp_impl->vlp_DescriptorManager = make_value<DescriptorHeapManager>(GetThis<GraphicDevice_Dx12>());
	m_uqp_impl->vlp_DescriptorManager->Initialize(*m_uqp_impl->cmp_device.Get());

	m_uqp_impl->vlp_pipelineStateManager = ObjectFactory::Create<PipelineStateManager>(GetThis<GraphicDevice_Dx12>());
	m_uqp_impl->vlp_rootSignatureManager= ObjectFactory::Create<RootSignatureManager>(GetThis<GraphicDevice_Dx12>());
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::Release()
{
	m_uqp_impl->vlp_DescriptorManager->Release();
	m_uqp_impl->vlp_pipelineStateManager->Release();
	m_uqp_impl->vlp_rootSignatureManager->Release();

	{
		const UINT64 fence = m_uqp_impl->fenceValue;
		const UINT64 lastCompletedFence = m_uqp_impl->cmp_fence->GetCompletedValue();
		(m_uqp_impl->cmp_commandQueue->Signal(m_uqp_impl->cmp_fence.Get(), m_uqp_impl->fenceValue));
		m_uqp_impl->fenceValue++;
		if (lastCompletedFence < fence)
		{
			m_uqp_impl->cmp_fence->SetEventOnCompletion(fence, m_uqp_impl->fenceEvent);
			WaitForSingleObject(m_uqp_impl->fenceEvent, INFINITE);
		}
		CloseHandle(m_uqp_impl->fenceEvent);
	}
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetWindow(std::int64_t arg_handle, std::int32_t arg_width, std::int32_t arg_height)
{
	auto depthData = make_value<ImageFileIO::TextureResourceData>();
	depthData->pixSize = sizeof(float);
	depthData->width = arg_width;
	depthData->height = arg_height;
	auto ds = ObjectFactory::Create<Resource_Texture_Dx12_DepthStencil>(depthData, GetThis<GraphicDevice_Dx12>());
	m_defaultDepthStencil = ds;
	m_uqp_impl->defaultDSVHandle = ds->GetHandle();
	m_uqp_impl->scissorRect.left = 0;
	m_uqp_impl->scissorRect.right = arg_width;
	m_uqp_impl->scissorRect.top = 0;
	m_uqp_impl->scissorRect.bottom =arg_height;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearDepthStancil(const float arg_depth)
{
	m_uqp_impl->p_currentCommandList->ClearDepthStencilView(m_uqp_impl->defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, arg_depth, 0, 0, nullptr);

}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetTopologyType(const TopologyType arg_current_topologyType)
{
	if (m_current_topologyType == arg_current_topologyType) {
		return;
	}
	m_current_topologyType = arg_current_topologyType;
	GetCommandList().IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(m_current_topologyType));
}

ID3D12Device& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDevice()
{
	return *m_uqp_impl->cmp_device.Get();
}

HRESULT ButiEngine::ButiRendering::GraphicDevice_Dx12::CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidResource, void** ppvResource)
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_createCommitedResourece);
	return m_uqp_impl->cmp_device->CreateCommittedResource(pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
}

ID3D12CommandQueue& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandQueue()
{
	return *m_uqp_impl->cmp_commandQueue.Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator()
{
	return *m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get();	
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator(const std::uint32_t arg_index)
{
	return *m_uqp_impl->cmp_commandAllocator[arg_index].Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetBundleCommandAllocator()
{
	return *m_uqp_impl->cmp_bundleCommandAllocator.Get();
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandList()
{
	return *m_uqp_impl->p_currentCommandList;
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetUploadCommandList()
{
	return *m_uqp_impl->cmp_uploadCommandList.Get();
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetClearCommandList()
{
	throw new ButiException(L"ClearCommandは存在しません");
	return *m_uqp_impl->cmp_uploadCommandList.Get();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& arg_pipelineState)
{
	if (arg_pipelineState.Get() == m_uqp_impl->currentPipelineState) {
		return;
	}
	m_uqp_impl->currentPipelineState = arg_pipelineState.Get();
	CommandListHelper::BundleSetPipeLine(arg_pipelineState, GetCommandList());
}
ButiEngine::Value_weak_ptr< ButiEngine::ButiRendering::DescriptorHeapManager> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDescriptorHeapManager()
{
	return m_uqp_impl->vlp_DescriptorManager;
}

ButiEngine::ButiRendering::PipelineStateManager& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetPipelineStateManager()
{
	return *m_uqp_impl->vlp_pipelineStateManager;
}

ButiEngine::ButiRendering::RootSignatureManager& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRootSignatureManager()
{
	return *m_uqp_impl->vlp_rootSignatureManager;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::AddUploadResource(GPUResource* arg_resource)
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_uploadResource);
	m_uqp_impl->list_uploadResourcesBuffer.push_back(arg_resource);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceBufferMerge()
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_uploadResource);
	m_uqp_impl->list_uploadResources = m_uqp_impl->list_uploadResourcesBuffer;
	m_uqp_impl->list_uploadResourcesBuffer.Clear();
}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceRelease()
{
	for (auto itr = m_uqp_impl->list_befUploadResources.begin(); itr != m_uqp_impl->list_befUploadResources.end(); itr++) {
		(*itr)->UpdateResourceRelease();
	}
	m_uqp_impl->list_befUploadResources.Clear();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::AddOutputResource(GPUResource* arg_resource, const FileFormat arg_format, const std::string& arg_fileName)
{
	ImageFileOutputInfo info;
	info.p_outputResource = arg_resource;
	info.outputName = arg_fileName;
	info.format = arg_format;
	m_uqp_impl->vec_outputInfo.push_back(info);
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::UnSetCommandList()
{
	m_uqp_impl->p_currentCommandList = nullptr;
	m_uqp_impl->currentPipelineState = nullptr;
}

ID3D12Fence& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetFence()
{
	return *m_uqp_impl->cmp_fence.Get();
}

IDXGISwapChain& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetSwapChain()
{
	throw new ButiException(L"SwapChainは存在しません");
	IDXGISwapChain* p_d=nullptr;
	return *p_d;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::WaitGPU()
{
	if (m_uqp_impl->cmp_fence->GetCompletedValue() < m_uqp_impl->fenceValue)
	{
		auto hr = m_uqp_impl->cmp_fence->SetEventOnCompletion(m_uqp_impl->fenceValue, m_uqp_impl->fenceEvent);
		WaitForSingleObject(m_uqp_impl->fenceEvent, INFINITE);

		if (hr != S_OK) {
			auto reason = m_uqp_impl->cmp_device->GetDeviceRemovedReason();
			throw new ButiException(L"WaitGPU内での例外");
			std::int32_t i = 0;
		}
	}
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::Present()
{

	m_uqp_impl->currentPipelineState = nullptr;
	auto hr = m_uqp_impl->cmp_presentCommandList->Reset(m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get(), nullptr);
	SetCommandList(m_uqp_impl->cmp_presentCommandList.Get());
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		itr->p_outputResource->CopyForOutput();
	}
	hr = m_uqp_impl->cmp_presentCommandList->Close();
	InsertCommandList();

	if (m_uqp_impl->vec_drawCommandLists.GetSize()) {
		m_uqp_impl->cmp_commandQueue->ExecuteCommandLists((std::uint32_t)m_uqp_impl->vec_drawCommandLists.GetSize(), &(m_uqp_impl->vec_drawCommandLists[0]));

		const UINT64 fenceV = m_uqp_impl->fenceValue;
		auto hr = m_uqp_impl->cmp_commandQueue->Signal(m_uqp_impl->cmp_fence.Get(), fenceV);

		if (hr != S_OK) {
			auto reason = m_uqp_impl->cmp_device->GetDeviceRemovedReason();
			assert(0);
		}

	}
	WaitGPU();
	m_uqp_impl->fenceValue++;
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		auto res = itr->p_outputResource;
		res->OutputToMemory();
	}
	m_uqp_impl->vec_outputInfo.clear();
	m_uqp_impl->vec_drawCommandLists.Clear();

}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawStart()
{
	ID3D12DescriptorHeap* ppHeaps[] = { m_uqp_impl->vlp_DescriptorManager->GetDescriptorHeap().Get(), m_uqp_impl->vlp_DescriptorManager->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	m_uqp_impl->p_currentCommandList->SetDescriptorHeaps(heapCount, ppHeaps);

}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawEnd()
{
	InsertCommandList();

	m_uqp_impl->cmp_commandQueue->ExecuteCommandLists((std::uint32_t)m_uqp_impl->vec_drawCommandLists.GetSize(), &(m_uqp_impl->vec_drawCommandLists[0]));

	const UINT64 fenceV = m_uqp_impl->fenceValue;
	auto hr = m_uqp_impl->cmp_commandQueue->Signal(m_uqp_impl->cmp_fence.Get(), fenceV);

	if (hr != S_OK) {
		auto reason = m_uqp_impl->cmp_device->GetDeviceRemovedReason();
		assert(0);
	}
	WaitGPU();

	m_uqp_impl->fenceValue++;
	m_uqp_impl->vec_drawCommandLists.Clear();
	m_uqp_impl->list_renderTargetHandles.Clear();
	m_uqp_impl->currentDSV = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::StartGUICommand()
{
	CommandListHelper::Reset(m_uqp_impl->cmp_guiCommandList,*m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());
	SetCommandList(m_uqp_impl->cmp_guiCommandList.Get());
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::EndGUICommand()
{
	CommandListHelper::Close(m_uqp_impl->cmp_guiCommandList);
	InsertCommandList(); 
	DrawEnd();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::Reset()
{
	m_uqp_impl->cmp_commandAllocator[GetFrameIndex()]->Reset();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearWindow()
{
	
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList, const std::int32_t index)
{
	if (m_uqp_impl->p_currentCommandList) {
		throw ButiException(L"commandList is already seted.", L"if (!currentCommandList)", L"ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList");
	}
	m_uqp_impl->p_currentCommandList = arg_currentCommandList;
	m_current_topologyType = TopologyType::none;
	m_vlp_currentMesh = nullptr;
	m_vlp_currentMaterial = nullptr;
	m_map_currentConstancBuffer.clear();
	m_map_currentSamplerState.clear();
	m_list_currentTexture.Clear();
	m_currentMeshVertexType =  -1 ;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDefaultRenderTarget()
{

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_uqp_impl->cmp_renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += GetFrameIndex() * m_uqp_impl->renderTargetDescriptorSize;

	m_uqp_impl->currentDSV = &m_uqp_impl->defaultDSVHandle;

	m_uqp_impl->list_renderTargetHandles.Add(rtvHandle);

}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDefaultDepthStencil()
{
	m_uqp_impl->currentDSV = &m_uqp_impl->defaultDSVHandle;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::CommandList_SetScissorRect()
{
	m_uqp_impl->p_currentCommandList->RSSetScissorRects(1, &m_uqp_impl->scissorRect);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::CommandList_SetRenderTargetView()
{
	m_uqp_impl->p_currentCommandList->OMSetRenderTargets(m_uqp_impl->list_renderTargetHandles.GetSize(), &m_uqp_impl->list_renderTargetHandles.GetFront(), false, m_uqp_impl->currentDSV);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle, Value_ptr<IRenderTarget> arg_vlp_renderTarget)
{
	m_uqp_impl->list_renderTargetHandles.Add(arg_rtvHandle);
	m_uqp_impl->list_vlp_usedRenderTargets.Add(arg_vlp_renderTarget);
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::InsertCommandList()
{
	if (!m_uqp_impl->p_currentCommandList) {
		return;
	}
	m_uqp_impl->vec_drawCommandLists.push_back(m_uqp_impl->p_currentCommandList);
	m_uqp_impl->p_currentCommandList = nullptr;

	m_vlp_currentMesh = nullptr;
	m_vlp_currentMaterial = nullptr;
	m_map_currentConstancBuffer.clear();
	m_map_currentSamplerState.clear();
	m_list_currentTexture.Clear();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ResourceUpload()
{
	m_uqp_impl->list_befUploadResources = m_uqp_impl->list_uploadResources;
	if (!m_uqp_impl->list_uploadResources.GetSize()) {
		return;
	}
	CommandListHelper::Reset(m_uqp_impl->cmp_uploadCommandList,*m_uqp_impl->cmp_commandAllocator[GetFrameIndex()].Get());

	SetCommandList(m_uqp_impl->cmp_uploadCommandList.Get());

	for (auto itr : m_uqp_impl->list_uploadResources) {
		itr->ResourceUpdate();
	}
	CommandListHelper::Close(m_uqp_impl->cmp_uploadCommandList);
	InsertCommandList();
	m_uqp_impl->list_uploadResources.Clear();
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::PipelineClear()
{
	m_uqp_impl->vlp_pipelineStateManager->ClearPipelineState();
	m_uqp_impl->vlp_rootSignatureManager->Release();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRtvHeap() const
{
	return  m_uqp_impl->cmp_renderTargetDescriptorHeap;
}

ID3D12Resource& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetTextureUploadHeap()
{
	return *m_uqp_impl->cmp_resourceUploadHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRtvHandle() const
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += m_uqp_impl->frameIndex * m_uqp_impl->renderTargetDescriptorSize;
	return rtvHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDsvHandle() const
{
	return m_uqp_impl->defaultDSVHandle;
}

const D3D12_RECT& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetScissorRect() const
{
	return m_uqp_impl->scissorRect;
}

std::uint32_t ButiEngine::ButiRendering::GraphicDevice_Dx12::GetFrameCount() const
{
	return 1;
	
}

std::uint32_t ButiEngine::ButiRendering::GraphicDevice_Dx12::GetFrameIndex() const
{
	return 0;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE* arg_dsv, Value_ptr<IDepthStencil> arg_vlp_depthStencil)
{
	m_uqp_impl->currentDSV = arg_dsv;
	m_uqp_impl->list_vlp_usedDepthStencils.Add(arg_vlp_depthStencil);
}

D3D12_CPU_DESCRIPTOR_HANDLE* ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDepthStencil()
{
	if (m_uqp_impl->currentDSV) {
		return m_uqp_impl->currentDSV;
	}
	return &m_uqp_impl->defaultDSVHandle;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::DisSetDepthStencil()
{
	m_uqp_impl->currentDSV = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ResetPipeLine()
{
	m_uqp_impl->currentPipelineState = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetSamplerState(SamplerState arg_samplerState, const std::int32_t arg_index)
{

	if (m_map_currentSamplerState.count(arg_index)) {
		if (m_map_currentSamplerState.at(arg_index) == arg_samplerState) { return; }

		m_map_currentSamplerState.at(arg_index) = arg_samplerState;
	}
	else {
		m_map_currentSamplerState.emplace(arg_index, arg_samplerState);
	}
	GetCommandList().SetGraphicsRootDescriptorTable(arg_index, GetDescriptorHeapManager().lock()->GetSamplerHandle(static_cast<std::int32_t>(arg_samplerState)).GPUHandle);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::GraphicDevice_Dx12> ButiEngine::ButiRendering::CreateGraphicDeviceDx12(const bool arg_isWindowApp)
{
	if(arg_isWindowApp)
		return ObjectFactory::Create<GraphicDevice_Dx12_WithWindow>();
	else {
		return ObjectFactory::Create<GraphicDevice_Dx12>();
	}
}
