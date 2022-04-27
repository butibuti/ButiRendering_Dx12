#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_RealTimeMesh_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"

ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::Resource_RealTimeMesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}



void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::Draw(const std::uint32_t arg_vertexType)
{

	vwp_graphicDevice.lock()->GetCommandList().IASetVertexBuffers(0, 1, &vertexBufferView);
	vwp_graphicDevice.lock()->GetCommandList().IASetIndexBuffer(&indexBufferView);


}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::Initialize()
{
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer)
{
	vertexBuffer = arg_vertexBuffer;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBuffer)
{
	indexBuffer = arg_indexBuffer;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap)
{
	vertexBufferUploadHeap = arg_vertexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap)
{
	indexBufferUploadHeap = arg_indexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview)
{
	vertexBufferView = arg_vertexBufferview;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView)
{
	indexBufferView = arg_indexBufferView;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetVertexCount(const std::uint32_t arg_vertexCount)
{
	verticesCount = arg_vertexCount;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::SetIndexCount(const std::uint32_t arg_indexCount)
{
	indexCount = arg_indexCount;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::GetVertexBuffer()
{
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::GetIndexBuffer()
{
	return indexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::GetVertexBufferUploadHeap()
{
	return vertexBufferUploadHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::GetIndexBufferUploadHeap()
{
	return indexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::Update()
{
	vwp_graphicDevice.lock()->AddUploadResource(GetThis<GPUResource>().get());

	isDataRefresh = true;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::ResourceUpdate()
{
	if (!vlp_meshPrimitive) {
		return;
	}
	//頂点バッファの更新
	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = vlp_meshPrimitive->GetVertexData();
	vertexData.RowPitch = vlp_meshPrimitive->GetVertexCount() * vlp_meshPrimitive->GetVertexSize();
	vertexData.SlicePitch = vertexData.RowPitch;
	DeviceHelper::UpdateSubresources<1>(&vwp_graphicDevice.lock()->GetCommandList(), GetVertexBuffer().Get(), GetVertexBufferUploadHeap().Get(), 0, 0, 1, &vertexData);
	auto trans = ResourceBarrierHelper::GetResourceBarrierTransition(GetVertexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	//インデックスバッファの更新
	if (vlp_meshPrimitive->GetIndexCount() > 0) {
		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = vlp_meshPrimitive->GetIndexData();
		indexData.RowPitch = vlp_meshPrimitive->GetIndexCount()* sizeof(std::uint32_t);
		indexData.SlicePitch = indexData.RowPitch;
		DeviceHelper::UpdateSubresources<1>(&vwp_graphicDevice.lock()->GetCommandList(), GetIndexBuffer().Get(), GetIndexBufferUploadHeap().Get(), 0, 0, 1, &indexData);
		auto tr = ResourceBarrierHelper::GetResourceBarrierTransition(GetIndexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &tr);
	}

	isDataRefresh = false;
}

void ButiEngine::ButiRendering::Resource_RealTimeMesh_Dx12::UpdateResourceRelease()
{
}
