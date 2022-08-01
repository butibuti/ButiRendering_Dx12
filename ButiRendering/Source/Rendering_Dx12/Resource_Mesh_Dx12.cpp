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
		
		
	}
	if (map_vlp_meshPrimitive.begin()->second->GetIndexCount() > 0) {
		D3D12_SUBRESOURCE_DATA indexData = {};
		indexData.pData = map_vlp_meshPrimitive.begin()->second->GetIndexData();
		indexData.RowPitch = map_vlp_meshPrimitive.begin()->second->GetIndexCount() * sizeof(std::uint32_t);
		indexData.SlicePitch = indexData.RowPitch;
		DeviceHelper::UpdateSubresources<1>(&vwp_graphicDevice.lock()->GetCommandList(), GetIndexBuffer().Get(), GetIndexBufferUploadHeap().Get(), 0, 0, 1, &indexData);
		auto tr = ResourceBarrierHelper::GetResourceBarrierTransition(GetIndexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		vwp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &tr);
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
ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Mesh> ButiEngine::ButiRendering::CreateMesh(const std::string& arg_meshName, const List<ButiEngine::Value_ptr<ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	auto vlp_graphicDevice_Dx12 = arg_vwp_graphicDevice.lock()->GetThis<ButiRendering::GraphicDevice_Dx12>();
	auto output = ObjectFactory::Create<ButiRendering::Resource_Mesh_Dx12>(vlp_graphicDevice_Dx12);

	std::uint32_t vertexCount;
	std::uint32_t indexCount;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	D3D12_HEAP_PROPERTIES defDesc = HeapPropertiesHelper::GetHeapProp(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_HEAP_PROPERTIES updesc = HeapPropertiesHelper::GetHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	for (auto& vlp_primitive : arg_list_vlp_inputMeshData) {
		output->SetBackupData(vlp_primitive);

		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;


		std::uint32_t vertexBufferSize = (std::uint32_t)(vlp_primitive->GetVertexCount() * vlp_primitive->GetVertexSize());
		D3D12_RESOURCE_DESC vBuffDesc = ResourceDescHelper::GetBufferResourceDesc(vertexBufferSize);
		//頂点バッファの作成
		{
			vlp_graphicDevice_Dx12->CreateCommittedResource(
				&defDesc,
				D3D12_HEAP_FLAG_NONE,
				&vBuffDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&vertexBuffer));

			vlp_graphicDevice_Dx12->CreateCommittedResource(
				&updesc,
				D3D12_HEAP_FLAG_NONE,
				&vBuffDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vertexBufferUploadHeap));
			// Initialize the vertex buffer view.
			vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
			vertexBufferView.StrideInBytes = static_cast<std::uint32_t>(vlp_primitive->GetVertexSize());
			vertexBufferView.SizeInBytes = static_cast<std::uint32_t>(vertexBufferSize);
		}
		//頂点数の設定
		vertexCount = static_cast<std::uint32_t>(vlp_primitive->GetVertexCount());
		output->SetVertexBuffer(vlp_primitive->GetVertexType(), vertexBuffer, vertexBufferUploadHeap, vertexBufferView);
	}
	output->SetVertexCount(vertexCount);
	//インデックスバッファの作成
	std::uint32_t indexBufferSize = static_cast<std::uint32_t>(sizeof(std::uint32_t) * arg_list_vlp_inputMeshData.GetFront()->GetIndexCount());
	D3D12_RESOURCE_DESC indexBuffDesc = ResourceDescHelper::GetBufferResourceDesc(indexBufferSize);
	{
		vlp_graphicDevice_Dx12->CreateCommittedResource(
			&defDesc,
			D3D12_HEAP_FLAG_NONE,
			&indexBuffDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&indexBuffer));

		vlp_graphicDevice_Dx12->CreateCommittedResource(
			&updesc,
			D3D12_HEAP_FLAG_NONE,
			&indexBuffDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBufferUploadHeap));

		// Describe the index buffer view.
		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = indexBufferSize;

	}
	//インデックス数の設定
	indexCount = static_cast<std::uint32_t>(arg_list_vlp_inputMeshData.GetFront()->GetIndexCount());

	output->SetIndexBuffer(indexBuffer);
	output->SetIndexBufferUploadHeap(indexBufferUploadHeap);
	output->SetIndexBufferView(indexBufferView);
	output->SetIndexCount(indexCount);
	output->SetTagName(arg_meshName);
	vlp_graphicDevice_Dx12->AddUploadResource(output.get());

	return output;
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Mesh>  ButiEngine::ButiRendering::CreateMesh(const std::string& arg_meshName, const List<ButiEngine::Value_ptr<ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData, const BoxEightCorner& arg_boxEightCorner, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	auto mesh= CreateMesh(arg_meshName, arg_list_vlp_inputMeshData, arg_vwp_graphicDevice);
	mesh->SetBoxEightCorner(arg_boxEightCorner);
	return mesh;
}

