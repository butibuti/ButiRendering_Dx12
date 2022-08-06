#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"../../../Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"..\../../Header/Rendering_Dx12/DescriptorHeapManager.h"

ButiEngine::ButiRendering::DescriptorHeapManager::DescriptorHeapManager( Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice,const std::uint32_t arg_max  )
{
	vwp_graphicDevice= arg_vwp_graphicDevice;
	maxCbv = arg_max;
}

ButiEngine::ButiRendering::DescriptorHeapManager::~DescriptorHeapManager()
{
	std::int32_t i = 0;
}

void ButiEngine::ButiRendering::DescriptorHeapManager::Initialize(ID3D12Device& device)
{
	cbvSrvUavDescriptorHeap = DescriptorHeapHelper::CreateCbvSrvUavHeap(DescriptorHeapSize,device);
	samplerDescriptorHeap = DescriptorHeapHelper::CreateSamplerHeap(8,device);
	cbvSrvDescriptorHandleIncrementSize	=device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	samplerDescriptorHandleIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	

	auto SamplerDescriptorHandle = samplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto samplerSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	SamplerHelper::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::SamplerNone, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::PointWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::PointClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::AnisotropicWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::AnisotropicClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::ComparisonLinear, SamplerDescriptorHandle, device);

	cbvSize =((UINT64) maxCbv) * 0x100;

	D3D12_HEAP_PROPERTIES prop = { D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1 };
	D3D12_RESOURCE_DESC desc = { D3D12_RESOURCE_DIMENSION_BUFFER, 0, (UINT64)cbvSize, 1, 1, 1, DXGI_FORMAT_UNKNOWN, { 1, 0 }, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE };
	device.CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBufferUploadHeap));

	D3D12_RANGE readRange = {};
	HRESULT hr = constantBufferUploadHeap->Map(0, &readRange, (void**)&mappedConstantBuffer);

	vec_cbBackUpData.resize(maxCbv);
}



ButiEngine::ButiRendering::HandleInformation ButiEngine::ButiRendering::DescriptorHeapManager::GetSamplerHandle(const std::uint32_t arg_key)
{
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =	samplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr +=samplerDescriptorHandleIncrementSize * arg_key;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = samplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); 
	cpuHandle.ptr +=arg_key*samplerDescriptorHandleIncrementSize;
	HandleInformation out{ gpuHandle,cpuHandle,arg_key };
	return out;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::DescriptorHeapManager::GetDescriptorHeap()
{
	return cbvSrvUavDescriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::ButiRendering::DescriptorHeapManager::GetSamplerHeap()
{
	return samplerDescriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::DescriptorHeapManager::GetConstantBuffer()
{
	return constantBufferUploadHeap;
}

ButiEngine::ButiRendering::HandleInformation ButiEngine::ButiRendering::DescriptorHeapManager::CreateConstantBufferView(void* arg_p_value, const bool arg_isKeep, const std::int32_t arg_size )
{
	std::uint32_t sizeAligned = (arg_size + 0xff) & ~0xff, numRequired = sizeAligned / 0x100, top;

	auto handle = GetCurrentHandle(top, sizeAligned);

	std::lock_guard lock(m_mtx_memory);

	memcpy(mappedConstantBuffer + top, arg_p_value, arg_size);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	BackUpConstantBufferData backUpData = {};

	cbvDesc.BufferLocation = constantBufferUploadHeap->GetGPUVirtualAddress() + ((UINT64)top * 0x100);

	cbvDesc.SizeInBytes = sizeAligned;


	assert((cbvDesc.SizeInBytes & 0xff) == 0);

	vwp_graphicDevice.lock()->GetDevice().CreateConstantBufferView(&cbvDesc, handle.CPUHandle);
	auto hr = vwp_graphicDevice.lock()->GetDevice().GetDeviceRemovedReason();


	if (vec_cbBackUpData[top]) {
		delete vec_cbBackUpData[top];
	}

	BackUpConstantBufferData* backup = new BackUpConstantBufferData();
	*backup = { top,sizeAligned,handle.CPUHandle };

	vec_cbBackUpData[top] = backup;

	return handle;
}

void ButiEngine::ButiRendering::DescriptorHeapManager::ConstantBufferUpdate(void* arg_p_value, const std::uint32_t arg_index, const std::int32_t arg_size)
{
	memcpy(mappedConstantBuffer + arg_index, arg_p_value, arg_size);
}


ButiEngine::ButiRendering::HandleInformation ButiEngine::ButiRendering::DescriptorHeapManager::GetCurrentHandle(const std::int32_t arg_size)
{
	static std::uint32_t i = 0;
	return GetCurrentHandle(i, arg_size);
}

ButiEngine::ButiRendering::HandleInformation ButiEngine::ButiRendering::DescriptorHeapManager::GetCurrentHandle(std::uint32_t& ref_top, const std::int32_t arg_size)
{
	std::lock_guard lock(m_mtx_memory);
	std::uint32_t sizeAligned = arg_size, numRequired = sizeAligned / 0x100, top;
	bool isUseSpace = false;
	{
		if (vec_space.size()) {
			for (auto itr = vec_space.begin(), end = vec_space.end(); itr != end;) {
				if (itr->size >= numRequired) {
					isUseSpace = true;

					top = itr->index;
					itr->index + numRequired;
					itr->size -= numRequired;
					if (itr->size == 0) {
						itr=vec_space.erase(itr);
						end = vec_space.end();
						break;
					}
					else {
						itr++;
					}
				}
				else {
					itr++;
				}
			}
		}
	}

	if ((!isUseSpace) && index + numRequired > maxCbv) {
		AddHeapRange();
		top = index;
		index += numRequired;
		//throw ButiException(L"", L"", L"");
	}
	else if ((!isUseSpace)) {
		top = index;
		index += numRequired;
	}


	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = cbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += top * cbvSrvDescriptorHandleIncrementSize;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = cbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += top * cbvSrvDescriptorHandleIncrementSize;
	HandleInformation out{ gpuHandle,cpuHandle,top };
	ref_top = top;
	return out;
}

std::uint32_t ButiEngine::ButiRendering::DescriptorHeapManager::GetDescriptorHandleIncrementSize()
{
	return cbvSrvDescriptorHandleIncrementSize;
}

std::uint32_t ButiEngine::ButiRendering::DescriptorHeapManager::GetIndex()
{
	return index;
}

void ButiEngine::ButiRendering::DescriptorHeapManager::Release(const BlankSpace& arg_releaseSpace)
{
	vec_space.push_back(arg_releaseSpace);
}

void ButiEngine::ButiRendering::DescriptorHeapManager::Release()
{
	m_list_descriptorHeapUpdateListner.Clear();
	for (auto itr : vec_cbBackUpData) {
		delete	itr;
	}
}

void ButiEngine::ButiRendering::DescriptorHeapManager::RegistUpdateListner(Value_ptr<IDescriptorHeapUpdateListner> arg_listner)
{
	m_list_descriptorHeapUpdateListner.Add(arg_listner);
}

void ButiEngine::ButiRendering::DescriptorHeapManager::ReCreateConstantBuffer()
{
	std::reverse(vec_cbBackUpData.begin(), vec_cbBackUpData.end());
	for (auto itr : vec_cbBackUpData) {
		if (!itr) {
			continue;
		}

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};


		cbvDesc.BufferLocation = constantBufferUploadHeap->GetGPUVirtualAddress() + ((UINT64)itr->top * 0x100);

		cbvDesc.SizeInBytes = itr->sizeAligned;


		assert((cbvDesc.SizeInBytes & 0xff) == 0);

		vwp_graphicDevice.lock()
			->GetDevice().CreateConstantBufferView(&cbvDesc, itr->cpuHandle);
	}
	std::reverse(vec_cbBackUpData.begin(), vec_cbBackUpData.end());
}

void ButiEngine::ButiRendering::DescriptorHeapManager::AddHeapRange()
{
	std::cout << "AddHeapRange" << std::endl;
	maxCbv *= 2;
	if (maxCbv > DescriptorHeapSize)
		throw ButiException(L"", L"", L"");

	std::uint64_t expandSize = static_cast<std::uint64_t>(maxCbv) * 0x100;

	vec_cbBackUpData.resize(maxCbv);
	
	D3D12_HEAP_PROPERTIES prop = { D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1 };
	D3D12_RESOURCE_DESC desc = constantBufferUploadHeap->GetDesc();
	desc.Width = expandSize;

	vwp_graphicDevice.lock()->GetDevice().CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBufferUploadHeap));
	buffer* mapped ;
	D3D12_RANGE readRange = {};
	HRESULT hr = constantBufferUploadHeap->Map(0, &readRange, (void**)&mapped);

	//memcpy(mapped, mappedConstantBuffer, cbvSize);

	 

	mappedConstantBuffer = mapped;

	for (auto& listner : m_list_descriptorHeapUpdateListner) {
		listner->OnDescriptorHeapUpdate();
	}

	ReCreateConstantBuffer();
	cbvSize =static_cast<std::uint32_t>(expandSize);

}
