#include"stdafx.h"
#include"../../Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"../../Header/Rendering_Dx12/Resource_Texture_Dx12_RenderTarget.h"
#include"../../Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiUtil/ButiUtil/ImageFileIO.h"
ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::Resource_Texture_Dx12_RenderTarget(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData,const std::int32_t format ,Value_ptr<GraphicDevice> arg_graphicDevice)
	//:Resource_Texture_Dx12(std::vector<BYTE>(width*height*4,255),width,height,arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	image.vlp_imageData = arg_vlp_imageData;
	image.format =(DXGI_FORMAT) format;
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


		auto hr = vwp_graphicDevice.lock()->GetDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(renderTargetDescriptorHeap.GetAddressOf()));
		if (FAILED(hr))
		{
		}

	}


	// リソースの設定.
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width =image.vlp_imageData-> width;
	desc.Height = image.vlp_imageData->height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 0;
	desc.Format = (DXGI_FORMAT)format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	// クリア値の設定.
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = (DXGI_FORMAT)format;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	auto rtdhHandle = renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();


	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr= vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(texture.GetAddressOf()));



	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = (DXGI_FORMAT)format;
	vwp_graphicDevice.lock()->GetDevice().CreateRenderTargetView(texture.Get(), &rtvDesc, rtdhHandle);


	auto srvHandleInfo = vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetCurrentHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	vwp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		texture.Get(),
		&srvDesc,
		cpuHandle);


	//レンダーターゲットビューのハンドルを取得
	rtvHandle=CD3DX12_CPU_DESCRIPTOR_HANDLE (
		renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 0,
		vwp_graphicDevice.lock()->GetDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));


	currentState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	textureDataSize = image.vlp_imageData->pixSize * image.vlp_imageData->width * image.vlp_imageData->height;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::SetRenderTarget(Vector4& arg_clearColor)
{
	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		currentState,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	currentState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	if (!isCleared) {
		isCleared = true;
		vwp_graphicDevice.lock()->GetCommandList().ClearRenderTargetView(rtvHandle, arg_clearColor.GetData(), 0, nullptr);
	}
	vwp_graphicDevice.lock()->PushRenderTarget(rtvHandle);

}


void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::CreateTextureUploadHeap()
{

}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::Initialize()
{
	vwp_graphicDevice.lock()->AddUploadResource(&(*GetThis<Resource_Texture_Dx12>()));
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::DisSetRenderTarget()
{
	auto desc = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		currentState,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &desc);
	currentState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	if ((GameDevice::GetInput()->CheckKey(Keys::Enter) || isEditorViewed) && !isCopyCommanded) {
		//isCopyCommanded = true;
		//vwp_graphicDevice.lock()->AddOutputResource(GetThis<GPUResource>().get(), FileFormat::None, "");
	}
}
void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::CopyForOutput()
{
	if (!resourceUploadHeap[0]) {
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);
		D3D12_HEAP_PROPERTIES readbackHeapProperties;
		readbackHeapProperties.Type = D3D12_HEAP_TYPE_READBACK;
		readbackHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		readbackHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		readbackHeapProperties.CreationNodeMask = 1;
		readbackHeapProperties.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC readbackBufferDesc{ CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize) };
		auto hr = vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
			&readbackHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&readbackBufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&resourceUploadHeap[0])
		);
		hr = vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
			&readbackHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&readbackBufferDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&resourceUploadHeap[1])
		);

		destLocation[0].pResource = resourceUploadHeap[0].Get();
		destLocation[0].Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		destLocation[1].pResource = resourceUploadHeap[1].Get();
		destLocation[1].Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		auto print = D3D12_PLACED_SUBRESOURCE_FOOTPRINT();
		print.Offset = 0;
		print.Footprint.Depth = 1;
		print.Footprint.Width = image.vlp_imageData->width;
		print.Footprint.Height = image.vlp_imageData->height;
		print.Footprint.Format = image.format;
		print.Footprint.RowPitch = image.vlp_imageData->width * image.vlp_imageData->pixSize;
		destLocation[0].PlacedFootprint = print;
		destLocation[1].PlacedFootprint = print;

		srcLocation.pResource = texture.Get();
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		srcLocation.SubresourceIndex = 0;
	}
	{
		auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(), currentState, D3D12_RESOURCE_STATE_COPY_SOURCE);
		vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
		currentState = D3D12_RESOURCE_STATE_COPY_SOURCE;

		auto uploadTrans = CD3DX12_RESOURCE_BARRIER::Transition(resourceUploadHeap[currentWritingOutputBuffer].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_DEST);
		vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	}

	vwp_graphicDevice.lock()->GetCommandList().CopyTextureRegion(&destLocation[currentWritingOutputBuffer], 0, 0, 0, &srcLocation, nullptr);
	currentWritingOutputBuffer = !currentWritingOutputBuffer;
}


void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::UpdateResourceRelease()
{
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::ResourceUpdate()
{
	if (!dataRefresh) {
		return;
	}

	dataRefresh = false;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::Attach(std::int32_t slot)
{
	vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);

}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::ToPNG(const std::string& arg_textureFilePath)
{

	if (!resourceUploadHeap[0]) {
		return;
	}
	outputFileName = arg_textureFilePath;
	isFileOutput = true;
	resourceUploadHeap[0] = nullptr;
	//vwp_graphicDevice.lock()->AddOutputResource(GetThis<GPUResource>().get(), FileFormat::PNG, outputFileName);

	
}

ButiEngine::GUIWindowReaction ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::ShowTextureWindow(const std::string& arg_windowName, const std::int32_t winFrag)
{
	std::string windowName = arg_windowName;

	if (windowName.size() == 0) {
		windowName = textureFilePath;
	}
	GUIWindowReaction output;
	bool isOpen = true;
	GUI::Begin(windowName, &isOpen, winFrag);
	if (!isOpen) {
		output.isClosed = true;
	}

	auto w = GUI::GetWindowWidth();
	auto h = GUI::GetWindowHeight() - (GUI::GetFontSize() + GUI::GetFramePadding() * 2);

	auto pos = GUI::GetWindowPos();

	Vector2 imageSize;
	if (w > (h - 2) * image.vlp_imageData->width / image.vlp_imageData->height) {
		imageSize = Vector2((h - 2) * image.vlp_imageData->width / image.vlp_imageData->height, h - 2);
	}
	else {
		imageSize = Vector2(w, w * image.vlp_imageData->height / image.vlp_imageData->width);
	}
	Vector2 imagePos = Vector2((w - imageSize.x) * 0.5f, (h - 2 - imageSize.y) * 0.5f);
	if (GUI::IsWindowHovered()) {
		output.isHovered = true;
	}
	if (imagePos.y > 0) {
		GUI::Dummy(Vector2(0, imagePos.y));
	}
	if (imagePos.x > 0) {
		GUI::Dummy(Vector2(imagePos.x, 0));
		GUI::SameLine();
	}
	GUI::Texture(GetResourcePtr(), imageSize);

	GUI::End();

	return output;
}

void* ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetResourcePtr()
{
	return &handle;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::SetIsCleared(bool arg_isClear)
{
	isCleared = arg_isClear;
}

ButiEngine::Vector2 ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetSize()
{
	return Vector2(image.vlp_imageData->width,image.vlp_imageData->height);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::OutputToMemory()
{

	std::lock_guard lock(mtx_rawData);
	D3D12_RANGE readbackBufferRange{ 0, textureDataSize };
	auto hr = resourceUploadHeap[currentWritingOutputBuffer]->Map(0, &readbackBufferRange, &p_outputRawData[currentWritingOutputBuffer]);
	resourceUploadHeap[currentWritingOutputBuffer]->Unmap(0, &readbackBufferRange);
	isCopyCommanded = false;
	if (isFileOutput) {
		isFileOutput = false;
		ImageFileIO::OutputPNG(outputFileName, p_outputRawData[currentWritingOutputBuffer], image.vlp_imageData->width, image.vlp_imageData->height, 0, image.vlp_imageData->pixSize);
	}
	isRawDataChanged = true;
	cv_rawData[!currentWritingOutputBuffer].notify_one();
}
const unsigned char* ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetRawData() {
	return (const unsigned char*)p_outputRawData[!currentWritingOutputBuffer];
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::DataLock()
{
	if (!p_guard_rawData) {
		p_guard_rawData = new std::lock_guard(mtx_rawData);
	}

}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::DataUnlock()
{

	if (p_guard_rawData) {
		delete p_guard_rawData;
		p_guard_rawData = nullptr;
	}
}
void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::SetEditorViewed(const bool arg_isViewed)
{
	std::lock_guard<std::mutex> lock(mtx_rawData);
	isEditorViewed = arg_isViewed;
	isRawDataChanged = true;
	cv_rawData[currentWritingOutputBuffer].notify_all();
}
void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::Swap()
{
	currentWritingOutputBuffer = !currentWritingOutputBuffer;
}
void ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::Wait()
{
	std::unique_lock<std::mutex> lock(mtx_rawData);
	cv_rawData[currentWritingOutputBuffer].wait(lock, [this] {return isRawDataChanged; });
	isRawDataChanged = false;
}
std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetFormat() const
{
	return image.format;
}

std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetStride() const
{
	return image.vlp_imageData->width * image.vlp_imageData->pixSize;
}

std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_RenderTarget::GetDataSize() const
{
	return image.vlp_imageData->width * image.vlp_imageData->height * image.vlp_imageData->pixSize;
}

