#pragma once
#include"stdafx.h"
#include"../Resource_DynamicMesh.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"GraphicDevice_Dx12.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_DynamicMesh_Dx12 :public Resource_DynamicMesh
{
public:
	BUTIRENDERING_API Resource_DynamicMesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_DynamicMesh_Dx12();
	BUTIRENDERING_API void Draw(const std::uint32_t arg_vertexType)override;

	BUTIRENDERING_API void Initialize()override;

	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer);
	BUTIRENDERING_API void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indeexBuffer);
	BUTIRENDERING_API void SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap);
	BUTIRENDERING_API void SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap);

	BUTIRENDERING_API void SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview);
	BUTIRENDERING_API void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView);

	BUTIRENDERING_API void SetVertexCount(const std::uint32_t arg_vertexCount);
	BUTIRENDERING_API void SetIndexCount(const std::uint32_t arg_indexCount);

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBufferUploadHeap();

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferUploadHeap();

	BUTIRENDERING_API void Update()override;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;


	BUTIRENDERING_API void ResourceUpdate() override;
	BUTIRENDERING_API void UpdateResourceRelease()override;

};

}
}