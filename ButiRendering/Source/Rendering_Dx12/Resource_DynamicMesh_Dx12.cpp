#include "stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_DynamicMesh_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::Resource_DynamicMesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::Draw(const std::uint32_t arg_vertexType)
{
	vwp_graphicDevice.lock()->GetCommandList().IASetVertexBuffers(0, 1, &vertexBufferView);
	vwp_graphicDevice.lock()->GetCommandList().IASetIndexBuffer(&indexBufferView);
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::Initialize()
{
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer)
{
	vertexBuffer = arg_vertexBuffer;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBuffer)
{
	indexBuffer = arg_indexBuffer;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap)
{
	vertexBufferUploadHeap = arg_vertexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap)
{
	indexBufferUploadHeap = arg_indexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview)
{
	vertexBufferView = arg_vertexBufferview;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView)
{
	indexBufferView = arg_indexBufferView;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetVertexCount(const std::uint32_t arg_vertexCount)
{
	verticesCount = arg_vertexCount;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::SetIndexCount(const std::uint32_t arg_indexCount)
{
	indexCount = arg_indexCount;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::GetVertexBuffer()
{
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::GetIndexBuffer()
{
	return indexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::GetVertexBufferUploadHeap()
{
	return vertexBufferUploadHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::GetIndexBufferUploadHeap()
{
	return indexBufferUploadHeap;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::Update()
{
	vwp_graphicDevice.lock()->AddUploadResource(GetThis<GPUResource>().get());

	isDataRefresh = true;
}

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::ResourceUpdate()
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

void ButiEngine::ButiRendering::Resource_DynamicMesh_Dx12::UpdateResourceRelease()
{
}

ButiEngine::Value_ptr<ButiEngine::ButiRendering::IResource_Mesh> ButiEngine::ButiRendering::CreateDynamicMesh(const std::string& arg_meshName, const List<ButiEngine::Value_ptr<ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice)
{
	auto vlp_graphicDevice_Dx12 = arg_vwp_graphicDevice.lock()->GetThis<ButiRendering::GraphicDevice_Dx12>();
	auto output = ObjectFactory::Create<ButiRendering::Resource_DynamicMesh_Dx12>(vlp_graphicDevice_Dx12);

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
		output->SetVertexBuffer(vertexBuffer);
		output->SetVertexBufferUploadHeap(vertexBufferUploadHeap);
		output->SetVertexBufferView(vertexBufferView);
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