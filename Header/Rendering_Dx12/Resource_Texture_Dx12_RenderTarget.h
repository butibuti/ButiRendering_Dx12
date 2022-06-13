#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_Texture_Dx12_RenderTarget :public Resource_Texture_Dx12, public IRenderTarget
{
public:
	BUTIRENDERING_API static Value_ptr<Resource_Texture_Dx12_RenderTarget>Create(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, const std::int32_t format, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
	BUTIRENDERING_API Resource_Texture_Dx12_RenderTarget(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, const std::int32_t format, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
	BUTIRENDERING_API void SetRenderTarget(Vector4& arg_clearColor)override;
	BUTIRENDERING_API void CreateTextureUploadHeap()override;
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void DisSetRenderTarget()override;
	BUTIRENDERING_API void  CopyForOutput()override;
	BUTIRENDERING_API void UpdateResourceRelease()override;
	BUTIRENDERING_API void ResourceUpdate() override;
	BUTIRENDERING_API void Attach(std::int32_t slot)override;
	BUTIRENDERING_API void ToPNG(const std::string& arg_textureFilePath);
	BUTIRENDERING_API void* GetResourcePtr()override;
	BUTIRENDERING_API void SetIsCleared(bool arg_isClear)override;
	BUTIRENDERING_API Vector2 GetSize()override;
	BUTIRENDERING_API void OutputToMemory()override;
	BUTIRENDERING_API std::int32_t GetFormat()const override;
	BUTIRENDERING_API std::int32_t GetStride()const override;
	BUTIRENDERING_API std::int32_t GetDataSize()const override;
	BUTIRENDERING_API const unsigned char* GetRawData()override;
	BUTIRENDERING_API void DataLock()override;
	BUTIRENDERING_API void DataUnlock()override;
	BUTIRENDERING_API std::string GetName()const override;
private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderTargetDescriptorHeap;
	D3D12_RESOURCE_STATES currentState;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	std::string outputFileName;
	//void UpdateResourceRelease()override;
	//void ResourceUpdate() override;
	bool isCleared = false, isCopyCommanded = false;
	bool isFileOutput = false;

public:
	void SetEditorViewed(const bool arg_isViewed)override;
	bool GetEditorViewed()const override { return isEditorViewed; }
	BUTIRENDERING_API void Swap()override;
	BUTIRENDERING_API void Wait()override;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap[2];
	void* p_outputRawData[2];
	bool isEditorViewed = false, currentWritingOutputBuffer = false, isRawDataChanged = false;
	std::mutex mtx_rawData;
	std::condition_variable cv_rawData[2];
	std::int32_t textureDataSize;

	std::lock_guard<std::mutex>* p_guard_rawData;
	D3D12_TEXTURE_COPY_LOCATION destLocation[2], srcLocation;
};
}
}