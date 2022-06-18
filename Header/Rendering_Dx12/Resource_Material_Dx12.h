#pragma once
#include"stdafx.h"
#include"../Resource_Material.h"
#include"../../Header/Rendering_Dx12/CArrayBuffer_Dx12.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
class Resource_Material_Dx12 :public Resource_Material,public IDescriptorHeapUpdateListner
{
public:
	BUTIRENDERING_API Resource_Material_Dx12(const MaterialValue& arg_var, Value_weak_ptr<IResource_Texture> arg_texture, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	BUTIRENDERING_API Resource_Material_Dx12(const MaterialValue& arg_var, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_vlp_renderer)override;
	BUTIRENDERING_API void Update()override;
	BUTIRENDERING_API std::uint32_t GetTextureCount()const override;
	BUTIRENDERING_API void SetMaterialVariable(const MaterialValue& arg_var);
	BUTIRENDERING_API MaterialValue& GetMaterialVariable()override;
	BUTIRENDERING_API const MaterialValue& GetMaterialVariable()const override;
	BUTIRENDERING_API MaterialValue_Deferred GetMaterialDeferredValue()const override;
	BUTIRENDERING_API void SetMaterialIndex(const std::int32_t arg_index)override;
	BUTIRENDERING_API void OnDescriptorHeapUpdate()override;
private:
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Value_ptr< CBuffer<MaterialValue>> materialBuffer;
};

class Resource_MaterialList_Dx12 :public Resource_Material {
public:
	BUTIRENDERING_API Resource_MaterialList_Dx12(const MaterialValue& arg_var, const List< Value_weak_ptr<IResource_Texture>>& arg_list_texture, const List<Value_ptr<IResource_Material>>& arg_list_material, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice);
	BUTIRENDERING_API void Initialize()override;
	BUTIRENDERING_API void Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_vlp_renderer)override;
	BUTIRENDERING_API void Update()override;
	BUTIRENDERING_API std::uint32_t GetTextureCount()const override;
	BUTIRENDERING_API void SetMaterialVariable(const MaterialValue& arg_var);
	BUTIRENDERING_API MaterialValue& GetMaterialVariable()override;
	BUTIRENDERING_API const MaterialValue& GetMaterialVariable()const override;
	BUTIRENDERING_API MaterialValue_Deferred GetMaterialDeferredValue()const override;
	BUTIRENDERING_API void SetMaterialIndex(const std::int32_t arg_index)override;
	BUTIRENDERING_API void SetMaterialList(const List<Value_ptr<IResource_Material>>& arg_list_material)override;
private:
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	Value_ptr< CArrayBuffer_Dx12<MaterialValue_Deferred>> materialBuffer;
	List<Value_ptr<IResource_Material>> m_list_material;
};
}
}