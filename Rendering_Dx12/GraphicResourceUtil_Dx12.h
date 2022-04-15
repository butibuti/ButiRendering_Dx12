#ifndef GRAPHICRESUTIL_H
#define GRAPHICRESUTIL_H

#pragma once
#include"stdafx.h"
#include<d3d12.h>
namespace ButiEngine {
namespace PipelineStateHelper {
static inline Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3D12Device& arg_device) {

	Microsoft::WRL::ComPtr<ID3D12PipelineState> Ret;
	auto hr = arg_device.CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret));

	return Ret;
}


struct ButiD3DX12_BLEND_DESC : public D3D12_BLEND_DESC
{
	explicit ButiD3DX12_BLEND_DESC(const D3D12_BLEND_DESC& desc) :
		D3D12_BLEND_DESC(desc)
	{}
	explicit ButiD3DX12_BLEND_DESC()
	{
		AlphaToCoverageEnable = TRUE;
		IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			TRUE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (std::uint32_t i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			RenderTarget[i] = defaultRenderTargetBlendDesc;
	}
	ButiD3DX12_BLEND_DESC(const BlendMode& arg_blend)
	{
		AlphaToCoverageEnable = FALSE;
		IndependentBlendEnable = FALSE;
		D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc;

		switch (arg_blend) {
		case BlendMode::AlphaBlend:
			defaultRenderTargetBlendDesc = { TRUE, FALSE,
			D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_AND_REVERSE,
			D3D12_COLOR_WRITE_ENABLE_ALL, };
			break;
		case BlendMode::Addition:
			defaultRenderTargetBlendDesc = { TRUE, FALSE,
		D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL, };
			break;
		case BlendMode::Subtruction:
			defaultRenderTargetBlendDesc = { TRUE, FALSE,
		D3D12_BLEND_INV_SRC_COLOR, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL, };
			break;
		case BlendMode::Reverse:
			defaultRenderTargetBlendDesc = { TRUE, FALSE,
		D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ONE, D3D12_BLEND_OP_REV_SUBTRACT,
		D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_SUBTRACT,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL, };
			break;
		case BlendMode::NoBlend:
			//AlphaToCoverageEnable = TRUE;
			defaultRenderTargetBlendDesc = { FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ZERO, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL, };
			break;
		}
		for (std::uint32_t i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			RenderTarget[i] = defaultRenderTargetBlendDesc;
	}
};
static inline Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateDefault3D(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, std::vector<D3D12_INPUT_ELEMENT_DESC>& vec_inputLayout, Microsoft::WRL::ComPtr<ID3DBlob>& arg_vertexShaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& arg_pixelShaderBlob, Microsoft::WRL::ComPtr<ID3DBlob>& arg_geometryShaderBlob, std::vector<std::int32_t>& arg_vec_outputformat, const BlendMode arg_BlendMode, const TopologyType arg_topologyType, ID3D12Device& arg_device, const bool isDepth = true) {


	//ZeroMemory(&RetDesc, sizeof(RetDesc));
	RetDesc.InputLayout = { vec_inputLayout.data(),static_cast<std::uint32_t>(vec_inputLayout.size()) };
	RetDesc.pRootSignature = rootSignature.Get();
	RetDesc.VS =
	{
		reinterpret_cast<std::uint8_t*>(arg_vertexShaderBlob->GetBufferPointer()),
		arg_vertexShaderBlob->GetBufferSize()
	};
	RetDesc.PS =
	{
		reinterpret_cast<std::uint8_t*>(arg_pixelShaderBlob->GetBufferPointer()),
		arg_pixelShaderBlob->GetBufferSize()
	};
	if (arg_geometryShaderBlob) {
		RetDesc.GS = {
			reinterpret_cast<std::uint8_t*>(arg_geometryShaderBlob->GetBufferPointer()),
			arg_geometryShaderBlob->GetBufferSize()
		};
	}
	RetDesc.RasterizerState = arg_rasteriserDesc;
	RetDesc.BlendState = ButiD3DX12_BLEND_DESC(arg_BlendMode);
	RetDesc.DepthStencilState = D3D12_DEPTH_STENCIL_DESC{ true, D3D12_DEPTH_WRITE_MASK_ALL,
D3D12_COMPARISON_FUNC_LESS,false, D3D12_DEFAULT_STENCIL_READ_MASK, D3D12_DEFAULT_STENCIL_WRITE_MASK,
{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS },
{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS }, };


	if (!isDepth) {
		RetDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	}

	RetDesc.SampleMask = UINT_MAX;
	RetDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)arg_topologyType;

	auto size = arg_vec_outputformat.size();
	RetDesc.NumRenderTargets = size;
	for (std::int32_t i = 0; i < size; i++) {
		RetDesc.RTVFormats[i] = (DXGI_FORMAT)arg_vec_outputformat[i];
	}

	RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	RetDesc.SampleDesc.Count = 1;

	return CreateDirect(RetDesc, arg_device);
}


}


namespace CommandListHelper {
static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateSimple(ID3D12Device& arg_device, ID3D12CommandAllocator& arg_commandAllocator) {

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
	(arg_device.CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		&arg_commandAllocator,
		nullptr,
		IID_PPV_ARGS(&Ret)));
	return Ret;
}
static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateBundleCommandList(ID3D12Device& arg_device, ID3D12CommandAllocator& arg_commandAllocator) {

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
	auto hr = (arg_device.CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_BUNDLE,
		&arg_commandAllocator,
		nullptr,
		IID_PPV_ARGS(&Ret)));
	return Ret;
}

static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateDefault(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, ID3D12Device& arg_device, ID3D12CommandAllocator& arg_commandAllocator) {

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
	auto hr = (arg_device.CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		&arg_commandAllocator,
		pipelineState.Get(),
		IID_PPV_ARGS(&Ret)));
	return Ret;
}
static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCopy(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, ID3D12Device& arg_device, ID3D12CommandAllocator& arg_commandAllocator) {

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
	(arg_device.CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_COPY,
		&arg_commandAllocator,
		pipelineState.Get(),
		IID_PPV_ARGS(&Ret)));
	return Ret;
}

static inline  void Reset(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, ID3D12CommandAllocator& arg_commandAllocator) {

	commandList->Reset(&arg_commandAllocator, nullptr);

}
static inline  void Reset(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, ID3D12CommandAllocator& arg_commandAllocator) {

	auto hr = commandList->Reset(&arg_commandAllocator, pipelineState.Get());
	if (hr != S_OK) {
		throw ButiException(L"", L"", L"");
	}
}
static inline  void BundleReset(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, ID3D12CommandAllocator& arg_commandAllocator) {

	HRESULT hr;
	if (pipelineState)
		hr = commandList->Reset(&arg_commandAllocator, pipelineState.Get());
	else {
		hr = commandList->Reset(&arg_commandAllocator, nullptr);
	}
	if (hr != S_OK) {
		throw ButiException(L"", L"", L"");
	}
}
static inline  void BundleSetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, ID3D12GraphicsCommandList& commandList) {

	commandList.SetPipelineState(pipelineState.Get());

}
static inline  void Close(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) {
	auto hr = commandList->Close();
	if (hr != S_OK) {
		throw ButiException(L"", L"", L"");
	}
}

}

namespace DescriptorHeapHelper {
static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ID3D12Device& device) {

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Ret;
	auto hr = device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(Ret.GetAddressOf()));

	return Ret;
}
static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(const std::uint32_t arg_numDescriptorHeap, ID3D12Device& device) {
	//CbvSrvデスクプリタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
	CbvSrvHeapDesc.NumDescriptors = arg_numDescriptorHeap;
	CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	return CreateDirect(CbvSrvHeapDesc, device);
}
static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(const std::uint32_t arg_numDescriptorHeap, ID3D12Device& device) {
	//サンプラーデスクプリタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
	SamplerHeapDesc.NumDescriptors = arg_numDescriptorHeap;
	SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	return CreateDirect(SamplerHeapDesc, device);
}

}

namespace SamplerHelper {
static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle, ID3D12Device& device) {
	D3D12_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	switch (State) {
	case SamplerState::PointWrap:
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	case SamplerState::PointClamp:
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case SamplerState::LinearWrap:
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	case SamplerState::LinearClamp:
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case SamplerState::AnisotropicWrap:
		samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	case SamplerState::AnisotropicClamp:
		samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case SamplerState::ComparisonLinear:
		samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

		break;
	}



	device.CreateSampler(&samplerDesc, Handle);
}
}

namespace RootSignatureHelper {
static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc, ID3D12Device& arg_device) {

	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> Ret;
	D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

	arg_device.CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&Ret));
	return Ret;
}

static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbvMat(const std::uint32_t materialCount, const std::uint32_t srvCount, const std::uint32_t samplerCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc, ID3D12Device& arg_device) {



	std::vector< D3D12_DESCRIPTOR_RANGE> ranges;
	for (std::uint32_t i = 0; i < srvCount; i++) {
		D3D12_DESCRIPTOR_RANGE in;
		in.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		in.NumDescriptors = 1;
		in.BaseShaderRegister = i;
		in.RegisterSpace = 0;
		in.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		ranges.push_back(in);
	}
	for (std::uint32_t i = 0; i < samplerCount; i++) {

		D3D12_DESCRIPTOR_RANGE sampler;

		sampler.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		sampler.NumDescriptors = 1;
		sampler.BaseShaderRegister = i;
		sampler.RegisterSpace = 0;
		sampler.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		ranges.push_back(sampler);
	}
	D3D12_DESCRIPTOR_RANGE cbv;

	cbv.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbv.NumDescriptors = 1;
	cbv.BaseShaderRegister = 0;
	cbv.RegisterSpace = 0;
	cbv.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	ranges.push_back(cbv);

	for (std::uint32_t i = 0; i < materialCount; i++) {

		D3D12_DESCRIPTOR_RANGE in;
		in.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		in.NumDescriptors = 1;
		in.BaseShaderRegister = 1 + i;
		in.RegisterSpace = 0;
		in.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		ranges.push_back(in);
	}

	std::vector<D3D12_ROOT_PARAMETER> rootParameters;
	for (std::uint32_t i = 0; i < srvCount; i++) {
		D3D12_ROOT_PARAMETER in;
		in.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		in.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		in.DescriptorTable.NumDescriptorRanges = 1;
		in.DescriptorTable.pDescriptorRanges = &ranges[i];
		rootParameters.push_back(in);
	}

	for (std::uint32_t i = 0; i < samplerCount; i++) {
		D3D12_ROOT_PARAMETER in;
		in.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		in.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		in.DescriptorTable.NumDescriptorRanges = 1;
		in.DescriptorTable.pDescriptorRanges = &ranges[srvCount + i];
		rootParameters.push_back(in);
	}

	D3D12_ROOT_PARAMETER cbvParam;
	cbvParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	cbvParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	cbvParam.DescriptorTable.NumDescriptorRanges = 1;
	cbvParam.DescriptorTable.pDescriptorRanges = &ranges[srvCount + samplerCount];
	rootParameters.push_back(cbvParam);

	for (std::uint32_t i = 0; i < materialCount; i++) {
		D3D12_ROOT_PARAMETER in;
		in.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		in.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		in.DescriptorTable.NumDescriptorRanges = 1;
		in.DescriptorTable.pDescriptorRanges = &ranges[srvCount + samplerCount + 1 + i];
		rootParameters.push_back(in);
	}


	arg_rootSignatureDesc.NumParameters = rootParameters.size();
	arg_rootSignatureDesc.pParameters = rootParameters.data();
	arg_rootSignatureDesc.NumStaticSamplers = 0;
	arg_rootSignatureDesc.pStaticSamplers = nullptr;
	arg_rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	auto out = CreateDirect(arg_rootSignatureDesc, arg_device);
	return out;
}
}

namespace ResourceBarrierHelper {
static inline D3D12_RESOURCE_BARRIER GetResourceBarrierTransition(ID3D12Resource* arg_p_resource, const D3D12_RESOURCE_STATES arg_before, const D3D12_RESOURCE_STATES arg_after, const std::uint32_t arg_subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, const D3D12_RESOURCE_BARRIER_FLAGS arg_flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
{
	D3D12_RESOURCE_BARRIER output;
	output.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	output.Flags = arg_flags;
	output.Transition.pResource = arg_p_resource;
	output.Transition.StateBefore = arg_before;
	output.Transition.StateAfter = arg_after;
	output.Transition.Subresource = arg_subresource;
	return output;
}
}
namespace RasterizeDescHelper {
static inline D3D12_RASTERIZER_DESC GetDefaultRasterizerDesc() {
	D3D12_RASTERIZER_DESC output;

	output.FillMode = D3D12_FILL_MODE_SOLID;
	output.CullMode = D3D12_CULL_MODE_BACK;
	output.FrontCounterClockwise = FALSE;
	output.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	output.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	output.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	output.DepthClipEnable = TRUE;
	output.MultisampleEnable = FALSE;
	output.AntialiasedLineEnable = FALSE;
	output.ForcedSampleCount = 0;
	output.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return output;
}
}

namespace ResourceDescHelper {
static inline D3D12_RESOURCE_DESC GetBufferResourceDesc(const UINT64 arg_width, const D3D12_RESOURCE_FLAGS arg_flags = D3D12_RESOURCE_FLAG_NONE, const UINT64 arg_alignment = 0) {
	D3D12_RESOURCE_DESC output;
	output.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	output.Alignment = arg_alignment;
	output.Width = arg_width;
	output.Height = 1;
	output.DepthOrArraySize = 1;
	output.MipLevels = 1;
	output.Format = DXGI_FORMAT_UNKNOWN;
	output.SampleDesc.Count = 1;
	output.SampleDesc.Quality = 0;
	output.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	output.Flags = arg_flags;

	return output;
}
}

namespace HeapPropertiesHelper {
static inline D3D12_HEAP_PROPERTIES GetHeapProp(const D3D12_HEAP_TYPE arg_type, const std::uint32_t arg_creationNodeMask = 1, const	std::uint32_t arg_nodeMask = 1) {
	D3D12_HEAP_PROPERTIES output;

	output.Type = arg_type;
	output.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	output.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	output.CreationNodeMask = arg_creationNodeMask;
	output.VisibleNodeMask = arg_nodeMask;

	return output;
}
}
namespace DeviceHelper {
static inline std::uint64_t GetRequiredIntermediateSize(ID3D12Resource* arg_p_DestinationResource,ID3D12Device& arg_device, const std::uint32_t arg_FirstSubresource, const std::uint32_t arg_NumSubresources) {

	D3D12_RESOURCE_DESC Desc = arg_p_DestinationResource->GetDesc();
	UINT64 RequiredSize = 0;

	arg_device.GetCopyableFootprints(&Desc, arg_FirstSubresource, arg_NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);

	return RequiredSize;
}
inline void MemcpySubresource(	const D3D12_MEMCPY_DEST* arg_p_dest,	const D3D12_SUBRESOURCE_DATA* arg_p_src, const size_t arg_rowSizeInBytes, const std::uint32_t arg_numRows, const std::uint32_t arg_numSlices)
{
	for (std::uint32_t z = 0; z < arg_numSlices; ++z)
	{
		BYTE* pDestSlice = reinterpret_cast<BYTE*>(arg_p_dest->pData) + arg_p_dest->SlicePitch * z;
		const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(arg_p_src->pData) + arg_p_src->SlicePitch * z;
		for (std::uint32_t y = 0; y < arg_numRows; ++y)
		{
			memcpy(pDestSlice + arg_p_dest->RowPitch * y,
				pSrcSlice + arg_p_src->RowPitch * y,
				arg_rowSizeInBytes);
		}
	}
}
static inline std::uint64_t UpdateSubresources( ID3D12GraphicsCommandList* arg_p_cmdList, ID3D12Resource* arg_p_destinationResource,ID3D12Resource* arg_p_intermediate,
	 std::uint32_t arg_firstSubresource, std::uint32_t arg_numSubresources,UINT64 arg_requiredSize,const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* arg_p_layouts,const std::uint32_t* arg_p_numRows,const UINT64* arg_p_rowSizesInBytes,
const D3D12_SUBRESOURCE_DATA* pSrcData)
{
	// Minor validation
	D3D12_RESOURCE_DESC IntermediateDesc = arg_p_intermediate->GetDesc();
	D3D12_RESOURCE_DESC DestinationDesc = arg_p_destinationResource->GetDesc();
	if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
		IntermediateDesc.Width < arg_requiredSize + arg_p_layouts[0].Offset ||
		arg_requiredSize >(SIZE_T) - 1 ||
		(DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
			(arg_firstSubresource != 0 || arg_numSubresources != 1)))
	{
		return 0;
	}

	BYTE* pData;
	HRESULT hr = arg_p_intermediate->Map(0, NULL, reinterpret_cast<void**>(&pData));
	if (FAILED(hr))
	{
		return 0;
	}

	for (std::uint32_t i = 0; i < arg_numSubresources; ++i)
	{
		if (arg_p_rowSizesInBytes[i] > (SIZE_T)-1) return 0;
		D3D12_MEMCPY_DEST DestData = { pData + arg_p_layouts[i].Offset, arg_p_layouts[i].Footprint.RowPitch, arg_p_layouts[i].Footprint.RowPitch * arg_p_numRows[i] };
		DeviceHelper:: MemcpySubresource(&DestData, &pSrcData[i], (SIZE_T)arg_p_rowSizesInBytes[i], arg_p_numRows[i], arg_p_layouts[i].Footprint.Depth);
	}
	arg_p_intermediate->Unmap(0, NULL);

	if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		D3D12_BOX SrcBox;

		SrcBox.left = std::uint32_t(arg_p_layouts[0].Offset);
		SrcBox.top = 0;
		SrcBox.front = 0;
		SrcBox.right = std::uint32_t(arg_p_layouts[0].Offset + arg_p_layouts[0].Footprint.Width);
		SrcBox.bottom = 1;
		SrcBox.back = 1;
		arg_p_cmdList->CopyBufferRegion(
			arg_p_destinationResource, 0, arg_p_intermediate, arg_p_layouts[0].Offset, arg_p_layouts[0].Footprint.Width);
	}
	else
	{
		for (std::uint32_t i = 0; i < arg_numSubresources; ++i)
		{
			D3D12_TEXTURE_COPY_LOCATION Dst;
			Dst.pResource = arg_p_destinationResource;
			Dst.SubresourceIndex = i + arg_firstSubresource;
			Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			D3D12_TEXTURE_COPY_LOCATION Src;
			Src.pResource = arg_p_intermediate;
			Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			Src.PlacedFootprint = arg_p_layouts[i];
			arg_p_cmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
		}
	}
	return arg_requiredSize;
}


static inline std::uint64_t UpdateSubresources( ID3D12GraphicsCommandList* arg_p_cmdList, ID3D12Resource* arg_p_destinationResource, ID3D12Resource* arg_p_intermediate, const std::int64_t arg_intermediateOffset,
	const std::uint32_t arg_firstSubresource, const std::uint32_t arg_numSubresources, D3D12_SUBRESOURCE_DATA* arg_p_srcData)
{
	std::uint64_t RequiredSize = 0;
	std::uint64_t MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(std::uint32_t) + sizeof(UINT64)) * arg_numSubresources;
	if (MemToAlloc > SIZE_MAX)
	{
		return 0;
	}
	void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
	if (pMem == NULL)
	{
		return 0;
	}
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
	UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + arg_numSubresources);
	std::uint32_t* pNumRows = reinterpret_cast<std::uint32_t*>(pRowSizesInBytes + arg_numSubresources);

	D3D12_RESOURCE_DESC Desc = arg_p_destinationResource->GetDesc();
	ID3D12Device* pDevice;
	arg_p_destinationResource->GetDevice(__uuidof(*pDevice), reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, arg_firstSubresource, arg_numSubresources, arg_intermediateOffset, pLayouts, pNumRows, pRowSizesInBytes, &RequiredSize);
	pDevice->Release();

	std::uint64_t Result = DeviceHelper:: UpdateSubresources(arg_p_cmdList, arg_p_destinationResource, arg_p_intermediate, arg_firstSubresource, arg_numSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes, arg_p_srcData);
	HeapFree(GetProcessHeap(), 0, pMem);
	return Result;
}
template <std::uint32_t MaxSubresources>
static inline UINT64 UpdateSubresources(
	ID3D12GraphicsCommandList* arg_p_cmdList,ID3D12Resource* arg_p_destinationResource,ID3D12Resource* arg_p_intermediate,
	const UINT64 arg_intermediateOffset, const std::uint32_t arg_firstSubresource,const std::uint32_t arg_numSubresources,D3D12_SUBRESOURCE_DATA* arg_p_srcData)
{
	UINT64 RequiredSize = 0;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts[MaxSubresources];
	std::uint32_t NumRows[MaxSubresources];
	UINT64 RowSizesInBytes[MaxSubresources];

	D3D12_RESOURCE_DESC Desc = arg_p_destinationResource->GetDesc();
	ID3D12Device* pDevice;
	arg_p_destinationResource->GetDevice(__uuidof(*pDevice), reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, arg_firstSubresource, arg_numSubresources, arg_intermediateOffset, Layouts, NumRows, RowSizesInBytes, &RequiredSize);
	pDevice->Release();

	return DeviceHelper::UpdateSubresources(arg_p_cmdList, arg_p_destinationResource, arg_p_intermediate, arg_firstSubresource, arg_numSubresources, RequiredSize, Layouts, NumRows, RowSizesInBytes, arg_p_srcData);
}
}
}

#endif // !GRAPHICRESUTIL_H