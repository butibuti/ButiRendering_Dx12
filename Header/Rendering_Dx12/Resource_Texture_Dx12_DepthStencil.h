#ifndef RESOURCE_DEPTHSTENCIL_H
#define RESOURCE_DEPTHSTENCIL_H
#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class GraphicDevice;
class Resource_Texture_Dx12_DepthStencil :public Resource_Texture_Dx12, public IDepthStencil
{
public:
	BUTIRENDERING_API static ButiEngine::Value_ptr<IResource_Texture> Create(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
	BUTIRENDERING_API Resource_Texture_Dx12_DepthStencil(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
	BUTIRENDERING_API void CreateTextureUploadHeap()override;
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void UpdateResourceRelease()override;
	BUTIRENDERING_API void ResourceUpdate() override;
	BUTIRENDERING_API void Attach(std::int32_t slot)override;
	BUTIRENDERING_API void SetIsCleared(bool arg_isClear)override;
	BUTIRENDERING_API Vector2 GetSize()override;
	BUTIRENDERING_API void SetDepthStencilView() override;
	BUTIRENDERING_API void DisSetDepthStencil() override;
	BUTIRENDERING_API std::int32_t GetFormat()const override;
	BUTIRENDERING_API std::int32_t GetStride()const override;
	BUTIRENDERING_API std::int32_t GetDataSize()const override;
	BUTIRENDERING_API void OutputToMemory()override;
	BUTIRENDERING_API const unsigned char* GetRawData()override;
	BUTIRENDERING_API std::string GetName()const override;
	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle()const { return dsvHandle; }
private:
	D3D12_RECT     scissorRect;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	bool isCleared = false, isCopyCommanded = false;
	D3D12_RESOURCE_STATES m_currentResourceState;
public:
	void SetEditorViewed(const bool arg_isViewed)override { isEditorViewed = arg_isViewed; }
	bool GetEditorViewed()const override { return isEditorViewed; }
private:
	bool isEditorViewed = false;
};
}
}
#endif