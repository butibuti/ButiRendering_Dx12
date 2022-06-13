#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include "ButiRendering_Dx12\Header\Rendering_Dx12\Resource_Texture_Dx12_DepthStencil.h"
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::Create(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	return ObjectFactory::Create<Resource_Texture_Dx12_DepthStencil>(arg_vlp_imageData, arg_graphicDevice);
}
ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::Resource_Texture_Dx12_DepthStencil(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	image.vlp_imageData = arg_vlp_imageData;
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		// 深度ステンシルビュー用ディスクリプタヒープの設定.
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		// 深度ステンシルビュー用ディスクリプタヒープを生成.

		auto hr = vwp_graphicDevice.lock()->GetDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(depthStencilDescriptorHeap.GetAddressOf()));
		assert(hr == S_OK && "CreateDescriptorHeapエラー");
	}

	// ヒーププロパティの設定.
	D3D12_HEAP_PROPERTIES prop;
	prop.Type = D3D12_HEAP_TYPE_DEFAULT;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	// リソースの設定.
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = image.vlp_imageData->width;
	desc.Height =image.vlp_imageData->height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 0;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	// クリア値の設定.
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;
	// リソースを生成.
	auto hr = vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(texture.GetAddressOf()));

	// 深度ステンシルビューの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2D.MipSlice = 0;

	// 深度ステンシルビューを生成.
	vwp_graphicDevice.lock()->GetDevice().CreateDepthStencilView(texture.Get(), &dsvDesc, depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());



	auto srvHandleInfo = vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetCurrentHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
	resourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.PlaneSlice = 0;
	resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


	vwp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		texture.Get(),
		&resourceViewDesc,
		cpuHandle);

	//デプスステンシルビューのハンドルを取得
	dsvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);


	scissorRect.left = 0;
	scissorRect.right =image.vlp_imageData->width;
	scissorRect.top = 0;
	scissorRect.bottom =image.vlp_imageData->height;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::CreateTextureUploadHeap()
{

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);
	auto hr = vwp_graphicDevice.lock()->GetDevice().GetDeviceRemovedReason();
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	hr = vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		nullptr,
		IID_PPV_ARGS(&resourceUploadHeap));
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::Initialize()
{
	vwp_graphicDevice.lock()->AddUploadResource(&(*GetThis<Resource_Texture_Dx12>()));
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::UpdateResourceRelease()
{

	resourceUploadHeap = nullptr;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::ResourceUpdate()
{
	if (!dataRefresh) {
		return;
	}

	dataRefresh = false;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::Attach(std::int32_t slot)
{
	vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::SetIsCleared(bool arg_isClear)
{
	isCleared = arg_isClear;
}

ButiEngine::Vector2 ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetSize()
{
    return Vector2(image.vlp_imageData->width,image.vlp_imageData->height);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::SetDepthStencilView()
{

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	if (!isCleared) {
		isCleared = true;
		vwp_graphicDevice.lock()->GetCommandList().ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	vwp_graphicDevice.lock()->SetDepthStencilView(&dsvHandle);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::DisSetDepthStencil()
{

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),		
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	vwp_graphicDevice.lock()->DisSetDepthStencil();

	if (isEditorViewed) {
		vwp_graphicDevice.lock()->AddOutputResource(GetThis<GPUResource>().get(), FileFormat::None, "");
	}
}

std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetFormat() const
{
	return image.format;
}

std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetStride() const
{
	return image.vlp_imageData->width * image.vlp_imageData->pixSize;
}

std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetDataSize() const
{
	return image.vlp_imageData->width*image.vlp_imageData->height* image.vlp_imageData->pixSize;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::OutputToMemory()
{
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);
	D3D12_RANGE readbackBufferRange{ 0, uploadBufferSize };
	auto hr = resourceUploadHeap->Map(0, &readbackBufferRange, &image.p_resourceData);
	isCopyCommanded = false;
}

const unsigned char* ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetRawData()
{
	return (unsigned char*)image.p_resourceData;
}

std::string ButiEngine::ButiRendering::Resource_Texture_Dx12_DepthStencil::GetName() const
{
	return GetTexturePath();
}
