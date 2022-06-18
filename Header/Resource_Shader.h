#pragma once
#include"stdafx.h"
#include"ResourceInterface.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice;
class Resource_PixelShader :public IResource_PixelShader
{
public:
	void Initialize()override {}
	void PreInitialize()override {}
	virtual void Attach()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
	virtual std::vector<std::int32_t>& GetFormats() = 0;
};
class Resource_VertexShader :public IResource_VertexShader
{
public:
	void Initialize()override {}
	void PreInitialize()override {}
	virtual void Attach()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
};
class Resource_GeometryShader :public IResource_GeometryShader
{
public:
	void Initialize()override {}
	void PreInitialize()override {}
	virtual void Attach()const = 0;
	virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
};

class Resource_Shader :public IResource_Shader
{
public:
	BUTIRENDERING_API Resource_Shader(Value_ptr<IResource_VertexShader> arg_vlp_vertexShader, Value_ptr<IResource_PixelShader> arg_vlp_pixelShader, Value_ptr<IResource_GeometryShader> arg_vlp_geometryShader, const std::string& arg_shaderName);
	void Initialize()override {}
	void PreInitialize()override {}
	BUTIRENDERING_API virtual void Attach()const;

	BUTIRENDERING_API bool GetIsContainGeometryShader()const;
	BUTIRENDERING_API std::string GetShaderName()const;

	BUTIRENDERING_API Value_ptr<IResource_VertexShader> GetVertexShader()override;
	BUTIRENDERING_API Value_ptr<IResource_PixelShader> GetPixelShader()override;
	BUTIRENDERING_API Value_ptr<IResource_GeometryShader> GetGeometryShader()override;
private:
	Value_ptr<IResource_VertexShader> vlp_vertexShader;
	Value_ptr<IResource_PixelShader> vlp_pixelShader;
	Value_ptr<IResource_GeometryShader> vlp_geometryShader;
	std::string shaderName;
};
}
}