#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/DescriptorHeapManager.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include "ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Texture_Dx12.h"
#include"ButiUtil/ButiUtil/ImageFileIO.h"
#include"resource.h"
#include"ButiRendering_Dx12/Header/GetDLLInstance.h"
ButiEngine::ButiRendering::Resource_Texture_Dx12::Resource_Texture_Dx12(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	switch (arg_vlp_imageData->pixSize)
	{
	case 4:
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case 2:
		image.format = DXGI_FORMAT_R8G8_UNORM;
		break;
	case 1:
		image.format = DXGI_FORMAT_R8_UNORM;
		break;
	default:
		assert(0&&"フォーマットが不明");
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}
	image.vlp_imageData = arg_vlp_imageData;
	TexMetadata texMetadata;

	texMetadata.width = arg_vlp_imageData->width;
	texMetadata.height = arg_vlp_imageData->height;
	texMetadata.depth = 1;
	texMetadata.arraySize = 1;
	texMetadata.mipLevels = 1;
	texMetadata.miscFlags = 0;
	texMetadata.miscFlags2 = 0;
	texMetadata.format =image.format;
	texMetadata.dimension = TEX_DIMENSION_TEXTURE2D;


	CreateTexture(&image, 1, texMetadata);

	auto srvHandleInfo = vwp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetCurrentHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	//テクスチャのシェーダリソースビューを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//フォーマット
	srvDesc.Format = GetTextureDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = GetTextureDesc().MipLevels;
	//シェーダリソースビュー
	vwp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		GetTexture().Get(),
		&srvDesc,
		cpuHandle);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::Attach(std::int32_t slot)
{
	vwp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::CreateTexture(Image* srcImages, size_t nimages, const TexMetadata& metadata)
{
	if (metadata.IsVolumemap()) {
		throw ButiException(
			L"この形式には対応してません",
			L"if (metadata.IsVolumemap())",
			L"Resource_Texture_Dx12::CreateTexture()"
		);
	}

	std::unique_ptr<CUSTAM_SUBRESOURCE_DATA[]> initData(new (std::nothrow) CUSTAM_SUBRESOURCE_DATA[metadata.mipLevels * metadata.arraySize]);
	if (!initData) {
		throw ButiException(
			L"テクスチャデータの領域取得に失敗しました",
			L"if (!initData)",
			L"Resource_Texture_Dx12::CreateTexture()"
		);

	}


	size_t idx = 0;
	for (size_t item = 0; item < metadata.arraySize; ++item)
	{
		for (size_t level = 0; level < metadata.mipLevels; ++level)
		{
			size_t index = metadata.ComputeIndex(level, item, 0);
			if (index >= nimages) {
				throw ButiException(
					L"データのインデックスが範囲外です",
					L"if (index >= nimages)",
					L"Resource_Texture_Dx12::CreateTexture()"
				);

			}
			Image& img = srcImages[index];

			if (img.format != metadata.format) {
				throw ButiException(
					L"データのフォーマットが違います",
					L"if (img.format != metadata.format)",
					L"Resource_Texture_Dx12::CreateTexture()"
				);
			}
			if (!img.vlp_imageData->rawData) {
				throw ButiException(
					L"データのポインタが不正です",
					L"if (!img.pixels)",
					L"Resource_Texture_Dx12::CreateTexture()"
				);
			}
			if (idx >= (metadata.mipLevels * metadata.arraySize)) {
				throw ButiException(
					L"データの並びが不定です",
					L"if (idx >= (metadata.mipLevels * metadata.arraySize))",
					L"Resource_Texture_Dx12::CreateTexture()"
				);
			}
			++idx;
		}
	}

	DXGI_FORMAT tformat = metadata.format;

	switch (metadata.dimension)
	{
	case TEX_DIMENSION_TEXTURE1D:
	{
		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<std::uint32_t>(metadata.width);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	}
	break;

	case TEX_DIMENSION_TEXTURE2D:
	{

		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<std::uint32_t>(metadata.width);
		textureResDesc.Height = static_cast<std::uint32_t>(metadata.height);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (metadata.IsCubemap()) {
			textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		}
		else {
			textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
		}
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	}
	break;

	case TEX_DIMENSION_TEXTURE3D:
	{
		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<std::uint32_t>(metadata.width);
		textureResDesc.Height = static_cast<std::uint32_t>(metadata.height);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	}
	break;
	}

	texturePixelSize = image.vlp_imageData->pixSize;


	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE, &textureResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture));
	//

	dataRefresh = true;

}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::ToPNG(const std::string& arg_textureFilePath)
{
	ImageFileIO::OutputPNG(arg_textureFilePath, image.vlp_imageData->rawData, image.vlp_imageData->width, image.vlp_imageData->height, 0, 4);
}

void* ButiEngine::ButiRendering::Resource_Texture_Dx12::GetResourcePtr()
{
	return &(handle);
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::CreateTextureUploadHeap()
{
	const UINT64 uploadBufferSize = DeviceHelper::GetRequiredIntermediateSize(texture.Get(),vwp_graphicDevice.lock()->GetDevice(), 0, 1);
	auto hr = vwp_graphicDevice.lock()->GetDevice().GetDeviceRemovedReason();
	D3D12_RESOURCE_DESC buffDesc = ResourceDescHelper::GetBufferResourceDesc(uploadBufferSize);
	D3D12_HEAP_PROPERTIES heapProp = HeapPropertiesHelper::GetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	hr= vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buffDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resourceUploadHeap));

}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::UpdateResourceRelease()
{
	resourceUploadHeap = nullptr;
}

void ButiEngine::ButiRendering::Resource_Texture_Dx12::Initialize()
{
	dataRefresh = true;
	vwp_graphicDevice.lock()->AddUploadResource(&(*GetThis<Resource_Texture_Dx12>()));
}
Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_Texture_Dx12::GetTexture()
{
	return texture;
}
Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_Texture_Dx12::GetTextureUploadHeap()
{
	return resourceUploadHeap;
}
D3D12_RESOURCE_DESC ButiEngine::ButiRendering::Resource_Texture_Dx12::GetTextureDesc()
{
	return textureResDesc;
}
const unsigned char* ButiEngine::ButiRendering::Resource_Texture_Dx12::GetRawData()
{
	return image.vlp_imageData->rawData;
}
std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12::GetFormat() const
{
	return image.format;
}
std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12::GetStride() const
{
	return image.vlp_imageData->width*texturePixelSize;
}
std::int32_t ButiEngine::ButiRendering::Resource_Texture_Dx12::GetDataSize() const
{
	return image.vlp_imageData->width *image.vlp_imageData->height * texturePixelSize;
}
void ButiEngine::ButiRendering::Resource_Texture_Dx12::ResourceUpdate()
{
	if (!dataRefresh) {
		return;
	}
	CreateTextureUploadHeap();
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = image.vlp_imageData->rawData;
	textureData.RowPitch = image.vlp_imageData->rowPitch;
	textureData.SlicePitch = image.vlp_imageData->slicePitch;

	DeviceHelper::UpdateSubresources(&vwp_graphicDevice.lock()->GetCommandList(),
		texture.Get(),
		resourceUploadHeap.Get(),
		0, 0, 1, &textureData);

	auto trans = ResourceBarrierHelper::GetResourceBarrierTransition(
		texture.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(
		1,
		&trans
	);
	//image.pixels.clear();
	dataRefresh = false;
}
size_t __cdecl ButiEngine::ButiRendering::Resource_Texture_Dx12::TexMetadata::ComputeIndex(size_t mip, size_t item, size_t slice) const
{
	if (mip >= mipLevels)
		return size_t(-1);

	switch (dimension)
	{
	case TEX_DIMENSION_TEXTURE1D:
	case TEX_DIMENSION_TEXTURE2D:
		if (slice > 0)
			return size_t(-1);

		if (item >= arraySize)
			return size_t(-1);

		return (item * (mipLevels)+mip);

	case TEX_DIMENSION_TEXTURE3D:
		if (item > 0)
		{
			// No support for arrays of volumes
			return size_t(-1);
		}
		else
		{
			size_t index = 0;
			size_t d = depth;

			for (size_t level = 0; level < mip; ++level)
			{
				index += d;
				if (d > 1)
					d >>= 1;
			}

			if (slice >= d)
				return size_t(-1);

			index += slice;

			return index;
		}

	default:
		return size_t(-1);
	}
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::CreateTexture(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	return ObjectFactory::Create<Resource_Texture_Dx12>(arg_vlp_imageData, arg_vlp_graphicDevice);
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::CreateTextureFromPNG(const std::string& arg_filePath, Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	if (Util::ExistFile(arg_filePath)) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();
		ImageFileIO::InputImageFile(arg_filePath, *resourceData);
		return CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	else {
		return GetDebugTexture(arg_vlp_graphicDevice);
	}
}

const void* GetResource(std::int32_t arg_resourceID, std::int32_t& arg_output_size) {

	LPCTSTR pResourceName = MAKEINTRESOURCE(arg_resourceID);
	HRSRC hResource = FindResource(GetButiRenderingDLLHInstance(), pResourceName, L"PNG");

	const void* p_ResourceData = LockResource(LoadResource(GetButiRenderingDLLHInstance(), hResource));
	arg_output_size=SizeofResource(GetButiRenderingDLLHInstance(), hResource);

	return p_ResourceData;
}
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::MaterialIcon::GetMaterialTexture_file(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	Value_ptr<IResource_Texture> texture = nullptr;
	if (!texture) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();	
		std::int32_t dataSize;
		auto resourcePtr = GetResource(IDB_PNG2,dataSize );
		ImageFileIO::InputImage(resourcePtr,dataSize, *resourceData);
		texture = CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	return texture;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::MaterialIcon::GetMaterialTexture_folder(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	Value_ptr<IResource_Texture> texture = nullptr;
	if (!texture) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();
		std::int32_t dataSize;
		auto resourcePtr = GetResource(IDB_PNG3, dataSize);
		ImageFileIO::InputImage(resourcePtr, dataSize, *resourceData);
		texture = CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	return texture;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::MaterialIcon::GetMaterialTexture_folder_upArrow(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	Value_ptr<IResource_Texture> texture = nullptr;
	if (!texture) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();
		std::int32_t dataSize;
		auto resourcePtr = GetResource(IDB_PNG4, dataSize);
		ImageFileIO::InputImage(resourcePtr, dataSize, *resourceData);
		texture = CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	return texture;
}


ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::MaterialIcon::GetMaterialTexture_folder_plus(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	Value_ptr<IResource_Texture> texture = nullptr;
	if (!texture) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();
		std::int32_t dataSize;
		auto resourcePtr = GetResource(IDB_PNG5, dataSize);
		ImageFileIO::InputImage(resourcePtr, dataSize, *resourceData);
		texture = CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	return texture;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Texture> ButiEngine::ButiRendering::GetDebugTexture(Value_ptr<GraphicDevice> arg_vlp_graphicDevice)
{
	Value_ptr<IResource_Texture> texture = nullptr;
	if (!texture) {
		auto resourceData = ObjectFactory::Create<ImageFileIO::TextureResourceData>();
		std::int32_t dataSize;
		auto resourcePtr = GetResource(IDB_PNG1, dataSize);
		ImageFileIO::InputImage(resourcePtr, dataSize, *resourceData);
		texture = CreateTexture(resourceData, arg_vlp_graphicDevice);
	}
	return texture;
}

