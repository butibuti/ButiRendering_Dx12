#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/PipelineStateManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"TaskSystem.h"

ButiEngine::ButiRendering::GraphicDevice_Dx12::GraphicDevice_Dx12(Value_weak_ptr<IApplication> arg_vwp_application)
{
	vwp_application = arg_vwp_application;
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

			hr = D3D12CreateDevice(nullptr, levels[i], IID_PPV_ARGS(device.GetAddressOf()));
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
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

#ifdef _WINDOWGENERATE

	DXGI_SWAP_CHAIN_DESC swapChaindesc = {};
	swapChaindesc.BufferCount = FrameCount;
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
	hr = factory4->CreateSwapChain(commandQueue.Get(), &swapChaindesc, swapChain.GetAddressOf());
	assert(hr== S_OK);
	
	// To IDXGISwapChain3.
	hr = swapChain->QueryInterface(IID_PPV_ARGS(swapChain3.GetAddressOf()));
	assert(hr == S_OK);

	//Get FrameBufferIndex.
	frameIndex = swapChain3->GetCurrentBackBufferIndex();

	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(renderTargetDescripterHeap.GetAddressOf()));
	assert(hr == S_OK);

#endif // !_EDITORBUILD





	renderTargetDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(depthStencilDescriptorHeap.GetAddressOf()));
	assert(hr == S_OK);
	//Get DescriptorSize of DSV

	depthDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


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

		hr = device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(depthStencil.GetAddressOf()));
		assert(hr == S_OK);
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

#ifdef _WINDOWGENERATE
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	handle = renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();

#endif
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewdesc = {};
	renderTargetViewdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewdesc.Texture2D.MipSlice = 0;
	renderTargetViewdesc.Texture2D.PlaneSlice = 0;

	for (std::uint32_t i = 0; i < FrameCount; i++)
	{
		HRESULT hr;
#ifdef _WINDOWGENERATE

		// Get BackBuffer
		hr = swapChain3->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
		assert(hr == S_OK);
		// Create RTV
		device->CreateRenderTargetView(renderTargets[i].Get(), &renderTargetViewdesc, handle);
		handle.ptr += renderTargetDescriptorSize;
#else

#endif // !_EDITORBUILD

		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator[i].GetAddressOf()));
		assert(hr == S_OK);

	}
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(bundleCommandAllocator.GetAddressOf()));
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
	device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&clearRootSignature));
#ifdef _WINDOWGENERATE
	//for clear
	clearCommandList = CommandListHelper::CreateSimple(*device.Get(), *commandAllocator[frameIndex].Get());
	CommandListHelper::Close(clearCommandList);


#endif // !_EDITORBUILD

	//for presentBarrier
	presentCommandList = CommandListHelper::CreateSimple(*device.Get(), *commandAllocator[frameIndex].Get());
	CommandListHelper::Close(presentCommandList);
	//for upload 
	uploadCommandList=CommandListHelper::CreateSimple(*device.Get(), *commandAllocator[frameIndex].Get());
	CommandListHelper::Close(uploadCommandList);
	//for draw
	drawCommandList= CommandListHelper::CreateSimple(*device.Get(), *commandAllocator[frameIndex].Get());
	CommandListHelper::Close(drawCommandList);

	//for gui
	guiCommandList=CommandListHelper::CreateSimple(*device.Get(), *commandAllocator[frameIndex].Get());
	CommandListHelper::Close(guiCommandList);

	// シザー矩形を設定
	scissorRect.left = 0;
	scissorRect.right = vwp_application.lock()->GetWindow()->GetSize().x;
	scissorRect.top = 0;
	scissorRect.bottom = vwp_application.lock()->GetWindow()->GetSize().y;


	// フェンスを生成
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
	if (hr!=S_OK)
	{
		throw ButiException(L"フェンスの生成失敗");
	}

	// フェンスイベントを生成
	fenceValue = 1;
	fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (fenceEvent == nullptr)
	{
		throw ButiException(L"フェンスイベントの生成失敗");
	}

	vlp_descripterManager = make_value<DescriptorHeapManager>(GetThis<GraphicDevice_Dx12>());
	vlp_descripterManager->Initialize(*device.Get());

	vlp_pipelineStateManager = ObjectFactory::Create<PipelineStateManager>(GetThis<GraphicDevice_Dx12>());

	defaultDSVHandle = GetDsvHeap()->GetCPUDescriptorHandleForHeapStart();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::PreInitialize()
{
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::Release()
{
	vlp_descripterManager->Release();
	vlp_descripterManager = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearDepthStancil(const float arg_depth)
{
	p_currentCommandList->ClearDepthStencilView(defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, arg_depth, 0, 0, nullptr);

}

ID3D12Device& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDevice()
{
	return *device.Get();
}

HRESULT ButiEngine::ButiRendering::GraphicDevice_Dx12::CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, const IID& riidResource, void** ppvResource)
{
	std::lock_guard<std::mutex> lock(mtx_createCommitedResourece);
	return device->CreateCommittedResource(pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
}

ID3D12CommandQueue& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandQueue()
{
	return *commandQueue.Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator()
{
#ifdef _WINDOWGENERATE
	return *commandAllocator[frameIndex].Get();
#else
	return *commandAllocator[0].Get();
#endif // !_EDITORBUILD

	
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandAllocator(const std::uint32_t arg_index)
{
	return *commandAllocator[arg_index].Get();
}

ID3D12CommandAllocator& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetBundleCommandAllocator()
{
	return *bundleCommandAllocator.Get();
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetCommandList()
{
	return *p_currentCommandList;
}

ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetUploadCommandList()
{
	return *uploadCommandList.Get();
}

#ifdef _WINDOWGENERATE
ID3D12GraphicsCommandList& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetClearCommandList()
{
	return *clearCommandList.Get();
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
	if (arg_pipelineState.Get() == currentPipelineState) {
		return;
	}
	currentPipelineState = arg_pipelineState.Get();
	CommandListHelper::BundleSetPipeLine(arg_pipelineState, GetCommandList());
}

std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRootSignature(const std::wstring& Key)
{
	auto it = map_rootSignature.count(Key);
	if (!it) {
		return { nullptr, D3D12_ROOT_SIGNATURE_DESC() };
	}
	return map_rootSignature[Key];
}

ButiEngine::Value_weak_ptr< ButiEngine::ButiRendering::DescriptorHeapManager> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDescriptorHeapManager()
{
	return vlp_descripterManager;
}

ButiEngine::ButiRendering::PipelineStateManager& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetPipelineStateManager()
{
	return *vlp_pipelineStateManager;
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
	map_rootSignature[Key] = { rootsig ,arg_desc };
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::AddUploadResource(GPUResource* arg_resource)
{
	std::lock_guard<std::mutex> lock(mtx_uploadResource);
	vec_uploadResourcesBuffer.push_back(arg_resource);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceBufferMerge()
{
	std::lock_guard<std::mutex> lock(mtx_uploadResource);
	vec_uploadResources = vec_uploadResourcesBuffer;
	vec_uploadResourcesBuffer.clear();
}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::UploadResourceRelease()
{
	for (auto itr = vec_befUploadResources.begin(); itr != vec_befUploadResources.end(); itr++) {
		(*itr)->UpdateResourceRelease();
	}
	vec_befUploadResources.clear();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::AddOutputResource(GPUResource* arg_resource, const FileFormat arg_format, const std::string& arg_fileName)
{
	ImageFileOutputInfo info;
	info.p_outputResource = arg_resource;
	info.outputName = arg_fileName;
	info.format = arg_format;
	vec_outputInfo.push_back(info);
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::UnSetCommandList()
{
	p_currentCommandList = nullptr;
	currentPipelineState = nullptr;
}

ID3D12Fence& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetFence()
{
	return *fence.Get();
}

#ifdef _WINDOWGENERATE
IDXGISwapChain& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetSwapChain()
{
	return *swapChain3.Get();
}
#endif

void ButiEngine::ButiRendering::GraphicDevice_Dx12::WaitGPU()
{

	const UINT64 fenceV = fenceValue;
	auto hr = commandQueue->Signal(fence.Get(), fenceV);

	if (hr != S_OK) {
		auto reason = device->GetDeviceRemovedReason();
		std::int32_t i = 0;
	}
	fenceValue++;

	if (fence->GetCompletedValue() < fenceV)
	{
		hr = fence->SetEventOnCompletion(fenceV, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);

		if (hr != S_OK) {
			auto reason = device->GetDeviceRemovedReason();
			std::int32_t i = 0;
		}
	}
#ifdef _WINDOWGENERATE
frameIndex = swapChain3->GetCurrentBackBufferIndex();
#endif // !_EDITORBUILD

	

}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::Present()
{

	currentPipelineState = nullptr;
	auto hr = presentCommandList->Reset(commandAllocator[frameIndex].Get(), nullptr);
#ifdef _WINDOWGENERATE

	//プレゼント用のバリアを張る
	auto desc = ResourceBarrierHelper::GetResourceBarrierTransition(renderTargets[frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	presentCommandList->ResourceBarrier(1,
		&desc);

#endif
	SetCommandList(presentCommandList.Get());
	for (auto itr = vec_outputInfo.begin(), end = vec_outputInfo.end(); itr != end; itr++) {
		itr->p_outputResource->CopyForOutput();
	}
	hr = presentCommandList->Close();
	InsertCommandList();

	if (vec_drawCommandLists.size()) {
		commandQueue->ExecuteCommandLists((std::uint32_t)vec_drawCommandLists.size(), &(vec_drawCommandLists[0]));
	}
	
#ifdef _WINDOWGENERATE
	hr = swapChain3->Present(0, 0);
#endif // !_EDITORBUILD

	
	WaitGPU();
	for (auto itr = vec_outputInfo.begin(), end = vec_outputInfo.end(); itr != end; itr++) {
		auto res = itr->p_outputResource;
		res->OutputToMemory();
	}
	vec_outputInfo.clear();
	vec_drawCommandLists.clear();

}
void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawStart()
{
	ID3D12DescriptorHeap* ppHeaps[] = { vlp_descripterManager->GetDescriptorHeap().Get(), vlp_descripterManager->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	p_currentCommandList->SetDescriptorHeaps(heapCount, ppHeaps);

}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::DrawEnd()
{
	InsertCommandList();

	commandQueue->ExecuteCommandLists((std::uint32_t)vec_drawCommandLists.size(), &(vec_drawCommandLists[0]));

	WaitGPU();
	vec_drawCommandLists.clear();
	vec_renderTargetHandles.clear();
	currentDSV = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::StartGUICommand()
{
	CommandListHelper::Reset(guiCommandList,*commandAllocator[frameIndex].Get());
	SetCommandList(guiCommandList.Get());
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::EndGUICommand()
{
	CommandListHelper::Close(guiCommandList);
	InsertCommandList(); 
	DrawEnd();
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ClearWindow()
{
	commandAllocator[frameIndex]->Reset();
#ifdef _WINDOWGENERATE
	clearCommandList->Reset(commandAllocator[frameIndex].Get(), nullptr);

	clearCommandList->SetGraphicsRootSignature(clearRootSignature.Get());


	D3D12_RESOURCE_BARRIER desc = ResourceBarrierHelper::GetResourceBarrierTransition(renderTargets[frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	clearCommandList->ResourceBarrier(1,
		&desc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += frameIndex * renderTargetDescriptorSize;

	clearCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &defaultDSVHandle);


	clearCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	clearCommandList->ClearDepthStencilView(defaultDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	clearCommandList->Close();
	SetCommandList(clearCommandList.Get());
	InsertCommandList();
#endif // !_EDITORBUILD

}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList, const std::int32_t index)
{
	if (p_currentCommandList) {
		throw ButiException(L"commandList is already seted.", L"if (!currentCommandList)", L"ButiEngine::ButiRendering::GraphicDevice_Dx12::SetCommandList");
	}
	p_currentCommandList = arg_currentCommandList;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDefaultRenderTarget()
{

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += GetFrameIndex() * renderTargetDescriptorSize;

	currentDSV = &defaultDSVHandle;

	vec_renderTargetHandles.push_back(rtvHandle);

}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDefaultDepthStencil()
{
	currentDSV = &defaultDSVHandle;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::CommandList_SetScissorRect()
{
	p_currentCommandList->RSSetScissorRects(1, &scissorRect);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::CommandList_SetRenderTargetView()
{
	p_currentCommandList->OMSetRenderTargets(vec_renderTargetHandles.size(), vec_renderTargetHandles.data(), false, currentDSV);
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle)
{
	vec_renderTargetHandles.push_back(arg_rtvHandle);
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::InsertCommandList()
{
	if (!p_currentCommandList) {
		return;
	}
	vec_drawCommandLists.push_back(p_currentCommandList);
	p_currentCommandList = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ResourceUpload()
{
	vec_befUploadResources = vec_uploadResources;
	if (!vec_uploadResources.size()) {
		return;
	}
	CommandListHelper::Reset(uploadCommandList,*commandAllocator[frameIndex].Get());

	SetCommandList(uploadCommandList.Get());

	for (auto itr = vec_uploadResources.begin(); itr != vec_uploadResources.end(); itr++) {
		(*itr)->ResourceUpdate();
	}
	CommandListHelper::Close(uploadCommandList);
	InsertCommandList();
	vec_uploadResources.clear();
}


void ButiEngine::ButiRendering::GraphicDevice_Dx12::PipelineClear()
{
	vlp_pipelineStateManager->ClearPipelineState();
	map_rootSignature.clear();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRtvHeap() const
{
	return  renderTargetDescripterHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDsvHeap() const
{
	return depthStencilDescriptorHeap;
}

ID3D12Resource& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetTextureUploadHeap()
{
	return *resourceUploadHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE ButiEngine::ButiRendering::GraphicDevice_Dx12::GetRtvHandle() const
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += frameIndex * renderTargetDescriptorSize;
	return rtvHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDsvHandle() const
{
	return defaultDSVHandle;
}

const D3D12_RECT& ButiEngine::ButiRendering::GraphicDevice_Dx12::GetScissorRect() const
{
	return scissorRect;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE* arg_dsv)
{
	currentDSV = arg_dsv;
}

D3D12_CPU_DESCRIPTOR_HANDLE* ButiEngine::ButiRendering::GraphicDevice_Dx12::GetDepthStencil()
{
	if (currentDSV) {
		return currentDSV;
	}
	return &defaultDSVHandle;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::DisSetDepthStencil()
{
	currentDSV = nullptr;
}

void ButiEngine::ButiRendering::GraphicDevice_Dx12::ResetPipeLine()
{
	currentPipelineState = nullptr;
}