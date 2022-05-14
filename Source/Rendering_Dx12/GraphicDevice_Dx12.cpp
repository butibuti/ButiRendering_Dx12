#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"TaskSystem.h"
#include "..\..\Header\Rendering_Dx12\GraphicDevice_Dx12.h"


class ButiEngine::ButiRendering::GraphicDevice_Dx12::Impl {
public:
	D3D12_CPU_DESCRIPTOR_HANDLE* currentDSV = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE defaultDSVHandle;
	static const std::uint32_t FrameCount =
#ifdef _WINDOWGENERATE

		3;
#else
		1;
#endif // !_WINDOWGENERATE

	Microsoft::WRL::ComPtr<ID3D12Device> device;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> bundleCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> presentCommandList;
#ifdef _WINDOWGENERATE
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> clearCommandList;
#endif // !_WINDOWGENERATE



	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uploadCommandList;

	std::vector<GPUResource*> vec_uploadResources;
	std::vector<GPUResource*> vec_uploadResourcesBuffer;
	std::vector<GPUResource*> vec_befUploadResources;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> guiCommandList;

	Value_ptr<DescriptorHeapManager> vlp_descripterManager;

	Value_ptr<PipelineStateManager> vlp_pipelineStateManager;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> vec_renderTargetHandles;

	ID3D12GraphicsCommandList* p_currentCommandList = nullptr;
	ID3D12PipelineState* currentPipelineState = nullptr;

	std::vector< ID3D12CommandList*> vec_drawCommandLists;
	Microsoft::WRL::ComPtr<ID3D12Fence>  fence;

	std::map<std::wstring, std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC>> map_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap;
	D3D12_RECT scissorRect;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
	std::uint32_t depthDescriptorSize, renderTargetDescriptorSize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  renderTargetDescripterHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE ary_rtvHandle[FrameCount];


	Microsoft::WRL::ComPtr<ID3D12RootSignature>  clearRootSignature;

#ifdef _WINDOWGENERATE
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
	std::uint32_t frameIndex;
#else

	const std::uint32_t frameIndex = 0;
#endif // !_WINDOWGENERATE

	HANDLE  fenceEvent;
	UINT64  fenceValue;

	std::vector<ImageFileOutputInfo > vec_outputInfo;
	std::mutex mtx_uploadResource, mtx_createCommitedResourece;
};
ButiEngine::ButiRendering::GraphicDevice_Dx12::GraphicDevice_Dx12(Value_weak_ptr<IApplication> arg_vwp_application)
{
	vwp_application = arg_vwp_application;
	m_uqp_impl = std::make_unique<Impl>();
}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::Initialize()
{
	HRESULT hr;


	Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(factory4.GetAddressOf()));


	IDXGIAdapter1* p_adapter = nullptr;
	{

		std::vector<IDXGIAdapter1*> vec_p_adapters;

		for (std::int32_t i = 0; factory4->EnumAdapters1(i, &p_adapter) != DXGI_ERROR_NOT_FOUND; i++) {
			vec_p_adapters.push_back(p_adapter);
		}

		for (std::int32_t i = vec_p_adapters.size() - 1; i >= 0; i--) {
			DXGI_ADAPTER_DESC1 adapterDesc;
			vec_p_adapters.at(i)->GetDesc1(&adapterDesc);

			auto name = adapterDesc.Description;
			if (!StringHelper::Contains(name, L"Microsoft") && !StringHelper::Contains(name, L"Intel")) {
				p_adapter = vec_p_adapters.at(i);
				break;
			}
		}

		if (!p_adapter) {
			p_adapter = vec_p_adapters.at(0);
		}
	}

	D3D_FEATURE_LEVEL level;
	{

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		for (std::int32_t i = 0; i < sizeof(levels) / sizeof(D3D_FEATURE_LEVEL); i++) {

			hr = D3D12CreateDevice(nullptr, levels[i], IID_PPV_ARGS(m_uqp_impl->device.GetAddressOf()));
			if (hr == S_OK) {
				level = levels[i];
				break;
			}
		}
	}

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = m_uqp_impl->device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(m_uqp_impl->commandQueue.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = m_uqp_impl->FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

#ifdef _WINDOWGENERATE

	DXGI_SWAP_CHAIN_DESC swapChaindesc = {};
	swapChaindesc.BufferCount = m_uqp_impl->FrameCount;
	swapChaindesc.BufferDesc.Width = vwp_application.lock()->GetWindow()->GetSize().x;
	swapChaindesc.BufferDesc.Height = vwp_application.lock()->GetWindow()->GetSize().y;
	swapChaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChaindesc.OutputWindow = vwp_application.lock()->GetWindow()->GetHandle();
	swapChaindesc.Windowed = TRUE;
	swapChaindesc.SampleDesc.Count = 1;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	//Create SwapChain.
	hr = factory4->CreateSwapChain(m_uqp_impl->commandQueue.Get(), &swapChaindesc, swapChain.GetAddressOf());
	assert(hr== S_OK);
	
	// To IDXGISwapChain3.
	hr = swapChain->QueryInterface(IID_PPV_ARGS(m_uqp_impl->swapChain3.GetAddressOf()));
	assert(hr == S_OK);

	//Get FrameBufferIndex.
	m_uqp_impl->frameIndex = m_uqp_impl->swapChain3->GetCurrentBackBufferIndex();

	hr = m_uqp_impl->device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_uqp_impl->renderTargetDescripterHeap.GetAddressOf()));
	assert(hr == S_OK);

#endif // !_EDITORBUILD





	m_uqp_impl->renderTargetDescriptorSize = m_uqp_impl->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = m_uqp_impl->device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_uqp_impl->depthStencilDescriptorHeap.GetAddressOf()));
	assert(hr == S_OK);
	//Get DescriptorSize of DSV

	m_uqp_impl->depthDescriptorSize = m_uqp_impl->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	//Generate DSV
	{
		D3D12_HEAP_PROPERTIES prop;
		prop.Type = D3D12_HEAP_TYPE_DEFAULT;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 1;
		prop.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC desc;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = vwp_application.lock()->GetAppInitData()->windowWidth;
		desc.Height = vwp_application.lock()->GetAppInitData()->windowHeight;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 0;
		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		hr = m_uqp_impl->device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(m_uqp_impl->depthStencil.GetAddressOf()));
		assert(hr == S_OK);
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		m_uqp_impl->device->CreateDepthStencilView(m_uqp_impl->depthStencil.Get(), &dsvDesc, m_uqp_impl->depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

#ifdef _WINDOWGENERATE
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	handle = m_uqp_impl->renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();

#endif
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewdesc = {};
	renderTargetViewdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewdesc.Texture2D.MipSlice = 0;
	renderTargetViewdesc.Texture2D.PlaneSlice = 0;

	for (std::uint32_t i = 0; i < m_uqp_impl->FrameCount; i++)
	{
		HRESULT hr;
#ifdef _WINDOWGENERATE

		// Get BackBuffer
		hr = m_uqp_impl->swapChain3->GetBuffer(i, IID_PPV_ARGS(m_uqp_impl->renderTargets[i].GetAddressOf()));
		assert(hr == S_OK);
		// Create RTV
		m_uqp_impl->device->CreateRenderTargetView(m_uqp_impl->renderTargets[i].Get(), &renderTargetViewdesc, handle);
		handle.ptr += m_uqp_impl->renderTargetDescriptorSize;
#else

#endif // !_EDITORBUILD

		hr = m_uqp_impl->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_uqp_impl->commandAllocator[i].GetAddressOf()));
		assert(hr == S_OK);

	}
	hr = m_uqp_impl->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(m_uqp_impl->bundleCommandAllocator.GetAddressOf()));
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
	m_uqp_impl->device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_uqp_impl->clearRootSignature));
#ifdef _WINDOWGENERATE
	//for clear
	m_uqp_impl->clearCommandList = CommandListHelper::CreateSimple(*m_uqp_impl->device.Get(), *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(m_uqp_impl->clearCommandList);


#endif // !_EDITORBUILD

	//for presentBarrier
	m_uqp_impl->presentCommandList = CommandListHelper::CreateSimple(*m_uqp_impl->device.Get(), *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(m_uqp_impl->presentCommandList);
	//for upload 
	m_uqp_impl->uploadCommandList=CommandListHelper::CreateSimple(*m_uqp_impl->device.Get(), *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(m_uqp_impl->uploadCommandList);
	//for draw
	m_uqp_impl->drawCommandList= CommandListHelper::CreateSimple(*m_uqp_impl->device.Get(), *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(m_uqp_impl->drawCommandList);

	//for gui
	m_uqp_impl->guiCommandList=CommandListHelper::CreateSimple(*m_uqp_impl->device.Get(), *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	CommandListHelper::Close(m_uqp_impl->guiCommandList);

	// シザー矩形を設定
	m_uqp_impl->scissorRect.left = 0;
	m_uqp_impl->scissorRect.right = vwp_application.lock()->GetWindow()->GetSize().x;
	m_uqp_impl->scissorRect.top = 0;
	m_uqp_impl->scissorRect.bottom = vwp_application.lock()->GetWindow()->GetSize().y;


	// フェンスを生成
	hr = m_uqp_impl->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_uqp_impl->fence.GetAddressOf()));
	if (hr!=S_OK)
	{
		throw ButiException(L"フェンスの生成失敗");
	}

	// フェンスイベントを生成
	m_uqp_impl->fenceValue = 1;
	m_uqp_impl->fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (m_uqp_impl->fenceEvent == nullptr)
	{
		throw ButiException(L"フェンスイベントの生成失敗");
	}

	m_uqp_impl->vlp_descripterManager = make_value<DescriptorHeapManager>(GetThis<GraphicDevice_Dx12>());
	m_uqp_impl->vlp_descripterManager->Initialize(*m_uqp_impl->device.Get());

	m_uqp_impl->vlp_pipelineStateManager = ObjectFactory::Create<PipelineStateManager>(GetThis<GraphicDevice_Dx12>());

	m_uqp_impl->defaultDSVHandle = GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::Release()
{
	m_uqp_impl->vlp_descripterManager->Release();
	m_uqp_impl->vlp_descripterManager = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearDepthStancil(const float arg_depth)
{
	m_uqp_impl->p_currentCommandList->ClearDepthStencilView(m_uqp_impl->defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, arg_depth, 0, 0, nullptr);

}

ID3D12Device& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDevice()
{
	return *m_uqp_impl->device.Get();
}

HRESULT ButiEngine::ButiRendering::GraphicDevice_Dx12::CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidResource, void** ppvResource)
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_createCommitedResourece);
	return m_uqp_impl->device->CreateCommittedResource(pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
}

ID3D12CommandQueue& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandQueue()
{
	return *m_uqp_impl->commandQueue.Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator()
{
#ifdef _WINDOWGENERATE
	return *m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get();
#else
	return *m_uqp_impl->commandAllocator[0].Get();
#endif // !_EDITORBUILD

	
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator(const std::uint32_t arg_index)
{
	return *m_uqp_impl->commandAllocator[arg_index].Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetBundleCommandAllocator()
{
	return *m_uqp_impl->bundleCommandAllocator.Get();
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandList()
{
	return *m_uqp_impl->p_currentCommandList;
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetUploadCommandList()
{
	return *m_uqp_impl->uploadCommandList.Get();
}

#ifdef _WINDOWGENERATE
ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetClearCommandList()
{
	return *m_uqp_impl->clearCommandList.Get();
}
#endif // !_EDITORBUILD


Microsoft::WRL::ComPtr<ID3D12RootSignature> ButiEngine::ButiRendering::GraphicDevice_Dx12::CreateSrvSmpCbvMat(const std::uint32_t materialCount, const std::uint32_t srvCount, const std::uint32_t samplerCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc)
{

	auto Ret = GetRootSignature(L"SrvSmpCbvMat:" + std::to_wstring(materialCount) + L"srv:" + std::to_wstring(srvCount) + L"sampler:" + std::to_wstring(samplerCount));
	if (Ret.first) {
		arg_rootSignatureDesc = Ret.second;
		return Ret.first;
	}
	auto out = RootSignatureHelper::CreateSrvSmpCbvMat(materialCount, srvCount, samplerCount, arg_rootSignatureDesc, GetDevice());
	SetRootSignature(L"SrvSmpCbvMat:" + std::to_wstring(materialCount) + L"srv:" + std::to_wstring(srvCount) + L"sampler:" + std::to_wstring(samplerCount), out, arg_rootSignatureDesc);

	return out;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& arg_pipelineState)
{
	if (arg_pipelineState.Get() == m_uqp_impl->currentPipelineState) {
		return;
	}
	m_uqp_impl->currentPipelineState = arg_pipelineState.Get();
	CommandListHelper::BundleSetPipeLine(arg_pipelineState, GetCommandList());
}

std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRootSignature(const std::wstring& Key)
{
	auto it = m_uqp_impl->map_rootSignature.count(Key);
	if (!it) {
		return { nullptr, D3D12_ROOT_SIGNATURE_DESC() };
	}
	return m_uqp_impl->map_rootSignature[Key];
}

ButiEngine::Value_weak_ptr< ButiEngine::ButiRendering::DescriptorHeapManager> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDescriptorHeapManager()
{
	return m_uqp_impl->vlp_descripterManager;
}

ButiEngine::ButiRendering::PipelineStateManager& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetPipelineStateManager()
{
	return *m_uqp_impl->vlp_pipelineStateManager;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc)
{
	if (GetRootSignature(Key).first != nullptr) {
		throw ButiException(
			L"すでにそのルートシグネチャは存在します",
			Key,
			L"DeviceResources::SetRootSignature()"
		);
	}
	m_uqp_impl->map_rootSignature[Key] = { rootsig ,arg_desc };
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::AddUploadResource(GPUResource* arg_resource)
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_uploadResource);
	m_uqp_impl->vec_uploadResourcesBuffer.push_back(arg_resource);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceBufferMerge()
{
	std::lock_guard<std::mutex> lock(m_uqp_impl->mtx_uploadResource);
	m_uqp_impl->vec_uploadResources = m_uqp_impl->vec_uploadResourcesBuffer;
	m_uqp_impl->vec_uploadResourcesBuffer.clear();
}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceRelease()
{
	for (auto itr = m_uqp_impl->vec_befUploadResources.begin(); itr != m_uqp_impl->vec_befUploadResources.end(); itr++) {
		(*itr)->UpdateResourceRelease();
	}
	m_uqp_impl->vec_befUploadResources.clear();
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
	return *m_uqp_impl->fence.Get();
}

#ifdef _WINDOWGENERATE
IDXGISwapChain& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetSwapChain()
{
	return *m_uqp_impl->swapChain3.Get();
}
#endif

void ButiEngine::ButiRendering::GraphicDevice_Dx12::WaitGPU()
{

	const UINT64 fenceV = m_uqp_impl->fenceValue;
	auto hr = m_uqp_impl->commandQueue->Signal(m_uqp_impl->fence.Get(), fenceV);

	if (hr != S_OK) {
		auto reason = m_uqp_impl->device->GetDeviceRemovedReason();
		std::int32_t i = 0;
	}
	m_uqp_impl->fenceValue++;

	if (m_uqp_impl->fence->GetCompletedValue() < fenceV)
	{
		hr = m_uqp_impl->fence->SetEventOnCompletion(fenceV, m_uqp_impl->fenceEvent);
		WaitForSingleObject(m_uqp_impl->fenceEvent, INFINITE);

		if (hr != S_OK) {
			auto reason = m_uqp_impl->device->GetDeviceRemovedReason();
			std::int32_t i = 0;
		}
	}
#ifdef _WINDOWGENERATE
m_uqp_impl->frameIndex = m_uqp_impl->swapChain3->GetCurrentBackBufferIndex();
#endif // !_EDITORBUILD

	

}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::Present()
{

	m_uqp_impl->currentPipelineState = nullptr;
	auto hr = m_uqp_impl->presentCommandList->Reset(m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get(), nullptr);
#ifdef _WINDOWGENERATE

	//プレゼント用のバリアを張る
	auto desc = ResourceBarrierHelper::GetResourceBarrierTransition(m_uqp_impl->renderTargets[m_uqp_impl->frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_uqp_impl->presentCommandList->ResourceBarrier(1,
		&desc);

#endif
	SetCommandList(m_uqp_impl->presentCommandList.Get());
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		itr->p_outputResource->CopyForOutput();
	}
	hr = m_uqp_impl->presentCommandList->Close();
	InsertCommandList();

	if (m_uqp_impl->vec_drawCommandLists.size()) {
		m_uqp_impl->commandQueue->ExecuteCommandLists((std::uint32_t)m_uqp_impl->vec_drawCommandLists.size(), &(m_uqp_impl->vec_drawCommandLists[0]));
	}
	
#ifdef _WINDOWGENERATE
	hr = m_uqp_impl->swapChain3->Present(0, 0);
#endif // !_EDITORBUILD

	
	WaitGPU();
	for (auto itr = m_uqp_impl->vec_outputInfo.begin(), end = m_uqp_impl->vec_outputInfo.end(); itr != end; itr++) {
		auto res = itr->p_outputResource;
		res->OutputToMemory();
	}
	m_uqp_impl->vec_outputInfo.clear();
	m_uqp_impl->vec_drawCommandLists.clear();

}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawStart()
{
	ID3D12DescriptorHeap* ppHeaps[] = { m_uqp_impl->vlp_descripterManager->GetDescriptorHeap().Get(), m_uqp_impl->vlp_descripterManager->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	m_uqp_impl->p_currentCommandList->SetDescriptorHeaps(heapCount, ppHeaps);

}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawEnd()
{
	InsertCommandList();

	m_uqp_impl->commandQueue->ExecuteCommandLists((std::uint32_t)m_uqp_impl->vec_drawCommandLists.size(), &(m_uqp_impl->vec_drawCommandLists[0]));

	WaitGPU();
	m_uqp_impl->vec_drawCommandLists.clear();
	m_uqp_impl->vec_renderTargetHandles.clear();
	m_uqp_impl->currentDSV = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::StartGUICommand()
{
	CommandListHelper::Reset(m_uqp_impl->guiCommandList,*m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());
	SetCommandList(m_uqp_impl->guiCommandList.Get());
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::EndGUICommand()
{
	CommandListHelper::Close(m_uqp_impl->guiCommandList);
	InsertCommandList(); 
	DrawEnd();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::Reset()
{
	m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex]->Reset();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearWindow()
{
	m_uqp_impl->clearCommandList->Reset(m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get(), nullptr);

	m_uqp_impl->clearCommandList->SetGraphicsRootSignature(m_uqp_impl->clearRootSignature.Get());


	D3D12_RESOURCE_BARRIER desc = ResourceBarrierHelper::GetResourceBarrierTransition(m_uqp_impl->renderTargets[m_uqp_impl->frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_uqp_impl->clearCommandList->ResourceBarrier(1,
		&desc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_uqp_impl->renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += m_uqp_impl->frameIndex * m_uqp_impl->renderTargetDescriptorSize;

	m_uqp_impl->clearCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &m_uqp_impl->defaultDSVHandle);


	m_uqp_impl->clearCommandList->ClearRenderTargetView(rtvHandle, &clearColor.x, 0, nullptr);
	m_uqp_impl->clearCommandList->ClearDepthStencilView(m_uqp_impl->defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_uqp_impl->clearCommandList->Close();
	SetCommandList(m_uqp_impl->clearCommandList.Get());
	InsertCommandList();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList, const std::int32_t index)
{
	if (m_uqp_impl->p_currentCommandList) {
		throw ButiException(L"commandList is already seted.", L"if (!currentCommandList)", L"ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList");
	}
	m_uqp_impl->p_currentCommandList = arg_currentCommandList;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDefaultRenderTarget()
{

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_uqp_impl->renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += GetFrameIndex() * m_uqp_impl->renderTargetDescriptorSize;

	m_uqp_impl->currentDSV = &m_uqp_impl->defaultDSVHandle;

	m_uqp_impl->vec_renderTargetHandles.push_back(rtvHandle);

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
	m_uqp_impl->p_currentCommandList->OMSetRenderTargets(m_uqp_impl->vec_renderTargetHandles.size(), m_uqp_impl->vec_renderTargetHandles.data(), false, m_uqp_impl->currentDSV);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle)
{
	m_uqp_impl->vec_renderTargetHandles.push_back(arg_rtvHandle);
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::InsertCommandList()
{
	if (!m_uqp_impl->p_currentCommandList) {
		return;
	}
	m_uqp_impl->vec_drawCommandLists.push_back(m_uqp_impl->p_currentCommandList);
	m_uqp_impl->p_currentCommandList = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ResourceUpload()
{
	m_uqp_impl->vec_befUploadResources = m_uqp_impl->vec_uploadResources;
	if (!m_uqp_impl->vec_uploadResources.size()) {
		return;
	}
	CommandListHelper::Reset(m_uqp_impl->uploadCommandList,*m_uqp_impl->commandAllocator[m_uqp_impl->frameIndex].Get());

	SetCommandList(m_uqp_impl->uploadCommandList.Get());

	for (auto itr = m_uqp_impl->vec_uploadResources.begin(); itr != m_uqp_impl->vec_uploadResources.end(); itr++) {
		(*itr)->ResourceUpdate();
	}
	CommandListHelper::Close(m_uqp_impl->uploadCommandList);
	InsertCommandList();
	m_uqp_impl->vec_uploadResources.clear();
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::PipelineClear()
{
	m_uqp_impl->vlp_pipelineStateManager->ClearPipelineState();
	m_uqp_impl->map_rootSignature.clear();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRtvHeap() const
{
	return  m_uqp_impl->renderTargetDescripterHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDsvHeap() const
{
	return m_uqp_impl->depthStencilDescriptorHeap;
}

ID3D12Resource& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetTextureUploadHeap()
{
	return *m_uqp_impl->resourceUploadHeap.Get();
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
	{
		return m_uqp_impl-> FrameCount;
	}
}

std::uint32_t ButiEngine::ButiRendering::GraphicDevice_Dx12::GetFrameIndex() const
{
	{
#ifdef _WINDOWGENERATE
		return m_uqp_impl->frameIndex;
#else
		return 0;
#endif // !_WINDOWGENERATE

	}
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE* arg_dsv)
{
	m_uqp_impl->currentDSV = arg_dsv;
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