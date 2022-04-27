#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Mesh_Dx12.h"

ButiEngine::ButiRendering::Resource_Mesh_Dx12::Resource_Mesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice)

{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::Draw(const std::uint32_t arg_vertexType)
{
	// プリミティブトポロジーの設定.

	vwp_graphicDevice.lock()->GetCommandList().IASetVertexBuffers(0, 1, &map_vertexBuffer.at(arg_vertexType).vertexBufferView);
	vwp_graphicDevice.lock()->GetCommandList().IASetIndexBuffer(&indexBufferView);


}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::Initialize()
{
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetVertexBuffer(const std::uint32_t arg_vertexType, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferResource, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap, D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview)
{
	if (map_vertexBuffer.count(arg_vertexType)) {
		assert(0 && "追加済みの頂点タイプです");
		return;
	}
	map_vertexBuffer.emplace(arg_vertexType,VertexBuffer { arg_vertexBufferResource,arg_vertexBufferUploadHeap,arg_vertexBufferview });
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBuffer)
{
	indexBuffer = arg_indexBuffer;
}


void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap)
{
	assert(arg_indexBufferUploadHeap);
	indexBufferUploadHeap = arg_indexBufferUploadHeap;
}


void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView)
{
	indexBufferView = arg_indexBufferView;
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetVertexCount(const std::uint32_t arg_vertexCount)
{
	verticesCount = arg_vertexCount;
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::SetIndexCount(const std::uint32_t arg_indexCount)
{
	indexCount = arg_indexCount;
}


Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_Mesh_Dx12::GetIndexBuffer()
{
	return indexBuffer;
}


Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_Mesh_Dx12::GetIndexBufferUploadHeap()
{
	return indexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::ResourceUpdate()
{
	if (!map_vlp_meshPrimitive.size()) {
		return;
	}
	for (auto& prim:map_vlp_meshPrimitive)
	{
		//頂点バッファの更新
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = prim.second->GetVertexData();
		vertexData.RowPitch = prim.second->GetVertexCount() * prim.second->GetVertexSize();
		vertexData.SlicePitch = vertexData.RowPitch;
		DeviceHelper::UpdateSubresources<1>(&vwp_graphicDevice.lock()->GetCommandList(), GetVertexBuffer(prim.first).bufferResource.Get(), GetVertexBuffer(prim.first).vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
		auto trans = ResourceBarrierHelper::GetResourceBarrierTransition(GetVertexBuffer(prim.first).bufferResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
		//インデックスバッファの更新
		if (prim.second->GetIndexCount() > 0) {
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = prim.second->GetIndexData();
			indexData.RowPitch = prim.second->GetIndexCount() * sizeof(std::uint32_t);
			indexData.SlicePitch = indexData.RowPitch;
			DeviceHelper::UpdateSubresources<1>(&vwp_graphicDevice.lock()->GetCommandList(), GetIndexBuffer().Get(), GetIndexBufferUploadHeap().Get(), 0, 0, 1, &indexData);
			auto tr = ResourceBarrierHelper::GetResourceBarrierTransition(GetIndexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &tr);
		}
	}
	isDataRefresh = false;
}

void ButiEngine::ButiRendering::Resource_Mesh_Dx12::UpdateResourceRelease()
{
	indexBufferUploadHeap = nullptr;
	for (auto& vertexBuffer : map_vertexBuffer) {
		vertexBuffer.second.vertexBufferUploadHeap = nullptr;
	}
}