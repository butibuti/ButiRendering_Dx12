#pragma once
#include"stdafx.h"
#include"../Resource_Material.h"
struct D3D12_GPU_DESCRIPTOR_HANDLE;
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_Material_Dx12 :public Resource_Material,public IDescriptorHeapUpdateListner
{
public:
	BUTIRENDERING_API Resource_Material_Dx12(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_ptr<IResource_Texture>>& arg_list_texture, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void Attach()override;
	BUTIRENDERING_API void Update()override;
	BUTIRENDERING_API std::uint32_t GetTextureCount()const override;
	BUTIRENDERING_API void SetMaterialVariable(const MaterialValue& arg_var);
	BUTIRENDERING_API MaterialValue& GetMaterialVariable()override;
	BUTIRENDERING_API const MaterialValue& GetMaterialVariable()const override;
	BUTIRENDERING_API void SetMaterialIndex(const std::int32_t arg_index)override;
	BUTIRENDERING_API void OnDescriptorHeapUpdate()override; 
	Value_ptr<ICBuffer> GetMaterialCBuffer()const override { return m_materialBuffer; }
private:
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	Value_ptr< CBuffer<MaterialValue>> m_materialBuffer;
};

class Resource_MaterialList_Dx12 :public Resource_Material {
public:
	BUTIRENDERING_API Resource_MaterialList_Dx12(const MaterialValue& arg_var, Value_weak_ptr<IResource_Shader> arg_vlp_shader, const List< Value_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, const DrawSettings& arg_drawSettings, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void Attach()override;
	BUTIRENDERING_API void Update()override;
	BUTIRENDERING_API std::uint32_t GetTextureCount()const override;
	BUTIRENDERING_API void SetMaterialVariable(const MaterialValue& arg_var);
	BUTIRENDERING_API MaterialValue& GetMaterialVariable()override;
	BUTIRENDERING_API const MaterialValue& GetMaterialVariable()const override;
	BUTIRENDERING_API void SetMaterialIndex(const std::int32_t arg_index)override;
	BUTIRENDERING_API void SetMaterialList(const List<Value_ptr<IResource_Material>>& arg_list_material)override;
	bool IsAlpha()const override { return false; }
	Value_ptr<ICBuffer> GetMaterialCBuffer()const override { return m_materialBuffer; }
private:
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	Value_ptr<CBuffer< MaterialValue_Deferred>> m_materialBuffer;
	List<Value_ptr<IResource_Material>> m_list_material;
};
}
}