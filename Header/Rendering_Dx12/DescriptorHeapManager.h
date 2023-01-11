#ifndef DESCRIPTORHEAPMANAGER_H
#define DESCRIPTORHEAPMANAGER_H


#pragma once
#include<memory>
#include <commctrl.h> 
#include <d3d12.h>
#include<wrl.h>
#include"../ResourceInterface.h"
namespace ButiEngine {
namespace ButiRendering {
class GraphicDevice_Dx12;

struct BlankSpace;
struct HandleInformation {
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	std::uint32_t index;
};
struct BackUpConstantBufferData {
	std::uint32_t top;
	std::uint32_t sizeAligned;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};
class DescriptorHeapManager
{
public:
	BUTIRENDERING_API DescriptorHeapManager(Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice, const std::uint32_t arg_max = 512);
	BUTIRENDERING_API ~DescriptorHeapManager();
	BUTIRENDERING_API void Initialize(ID3D12Device& device);

	BUTIRENDERING_API HandleInformation GetSamplerHandle(const std::uint32_t key);

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSamplerHeap();
	
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetConstantBuffer();

	BUTIRENDERING_API void AddHeapRange();
	BUTIRENDERING_API HandleInformation CreateConstantBufferView(void* arg_p_value, const bool arg_isKeep, const std::int32_t arg_size = 0x100);
	BUTIRENDERING_API void ConstantBufferUpdate(void* arg_p_value, const std::int32_t arg_index, const std::int32_t arg_size = 0x100);
	//HandleInformation CreateShaderResourceView(Microsoft::WRL::ComPtr<ID3D12Resource> resource ,const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,const std::int32_t size );
	BUTIRENDERING_API HandleInformation GetCurrentHandle(const std::int32_t arg_size = 0x100);
	BUTIRENDERING_API HandleInformation GetCurrentHandle(std::uint32_t& arg_ref_top, const std::int32_t arg_size = 0x100);
	BUTIRENDERING_API std::uint32_t GetDescriptorHandleIncrementSize();
	BUTIRENDERING_API std::uint32_t GetIndex();
	BUTIRENDERING_API void Release(const BlankSpace& arg_releaseSpace);
	BUTIRENDERING_API void Release();
	BUTIRENDERING_API void RegistUpdateListner(Value_ptr<IDescriptorHeapUpdateListner> arg_listner);
private:
	BUTIRENDERING_API void ReCreateConstantBuffer();
	std::uint32_t cbvSrvDescriptorHandleIncrementSize = 0;
	std::uint32_t samplerDescriptorHandleIncrementSize = 0;
	std::uint32_t index = 0;
	std::uint32_t maxCbv = 0;
	std::uint32_t ApplicationKeepRange = 10;
	std::uint32_t cbvSize = 0;
	///デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvSrvUavDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> samplerDescriptorHeap;
	std::vector<BlankSpace> vec_space;
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBufferUploadHeap;
	struct buffer { char buf[0x100]; };

	buffer* mappedConstantBuffer = nullptr;
	std::vector<BackUpConstantBufferData*> vec_cbBackUpData;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	List<Value_ptr<IDescriptorHeapUpdateListner>> m_list_descriptorHeapUpdateListner;
	std::mutex m_mtx_memory;
};

}
}


#endif // !DESCRIPTORHEAPMANAGER_H