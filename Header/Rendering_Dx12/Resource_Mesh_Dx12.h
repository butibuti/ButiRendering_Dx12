#pragma once
#include"stdafx.h"
#include"../Resource_Mesh.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
#include"GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"../Resource_Mesh.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_Mesh_Dx12 :public Resource_Mesh
{
public:
	struct VertexBuffer {
		Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
	BUTIRENDERING_API Resource_Mesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API void Draw(const std::uint32_t arg_vertexType)override;

	BUTIRENDERING_API void Initialize()override;

	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API void SetVertexBuffer(const std::uint32_t arg_vertexType, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferResource, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap, D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview);
	BUTIRENDERING_API void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indeexBuffer);
	BUTIRENDERING_API void SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap);
	BUTIRENDERING_API void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView);

	BUTIRENDERING_API void SetVertexCount(const std::uint32_t arg_vertexCount);

	BUTIRENDERING_API void SetIndexCount(const std::uint32_t arg_indexCount);


	const VertexBuffer& GetVertexBuffer(std::uint32_t arg_vertexType) { return map_vertexBuffer.at(arg_vertexType); }

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();


	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferUploadHeap();

private:
	std::map<std::uint32_t, VertexBuffer> map_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;


	BUTIRENDERING_API void ResourceUpdate() override;
	BUTIRENDERING_API void UpdateResourceRelease()override;

};
}
}