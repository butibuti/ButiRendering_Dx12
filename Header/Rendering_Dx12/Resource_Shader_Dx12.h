#pragma once
#include"stdafx.h"
#include"../Resource_Shader.h"
#include"../ShaderReflection.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
namespace ShaderCompile_Dx12 {
static void Compile(const std::string& arg_source, const char* arg_filePath, Microsoft::WRL::ComPtr<ID3DBlob>& arg_out, const char* entryPoint, const char* shaderModel);
}

class Resource_PixelShader_Dx12 :public Resource_PixelShader
{
public:
	BUTIRENDERING_API Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath,Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_PixelShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_PixelShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetPixelShaderBlob();
	const std::string& GetName()const override { return m_shaderName; }
	std::int32_t GetConstantBufferCount() const override { return m_list_constBufferReflection.GetSize(); }
	const List<ConstantBufferReflection>& GetConstantBufferReflectionDatas() const override { return m_list_constBufferReflection; }
	const List<TextureReflection>& GetTextureReflectionDatas() const override { return m_list_textureReflection; }
	const List<SamplerReflection>& GetSamplerReflectionDatas() const override { return m_list_samplerReflection; }
	const ShaderStructReflection& GetInputLayout()const override { return m_inputLayoutReflection; }
	const ShaderStructReflection& GetOutputLayout()const override { return m_outputLayoutReflection; }
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size,const bool arg_isfxc);
	Microsoft::WRL::ComPtr<ID3DBlob> m_pixelShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	List<ConstantBufferReflection> m_list_constBufferReflection;
	List<TextureReflection> m_list_textureReflection;
	List<SamplerReflection> m_list_samplerReflection;
	ShaderStructReflection m_inputLayoutReflection, m_outputLayoutReflection;
	std::string m_shaderName;
};
class Resource_VertexShader_Dx12 :public Resource_VertexShader
{
public:
	BUTIRENDERING_API Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath,  Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_VertexShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName,Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_VertexShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetVertexShaderBlob();
	BUTIRENDERING_API const List<D3D12_INPUT_ELEMENT_DESC>& GetInputLayoutList()const;
	BUTIRENDERING_API std::uint32_t GetInputVertexType()const override;
	const std::string& GetName()const override { return m_shaderName; }
	std::int32_t GetConstantBufferCount() const override { return m_list_constBufferReflection.GetSize(); }
	const List<ConstantBufferReflection>& GetConstantBufferReflectionDatas() const override { return m_list_constBufferReflection; }
	const List<TextureReflection>& GetTextureReflectionDatas() const override { return m_list_textureReflection; }
	const List<SamplerReflection>& GetSamplerReflectionDatas() const override { return m_list_samplerReflection; }
	const ShaderStructReflection& GetInputLayout()const override { return m_inputLayoutReflection; }
	const ShaderStructReflection& GetOutputLayout()const override { return m_outputLayoutReflection; }
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const bool arg_isfxc);
	Microsoft::WRL::ComPtr<ID3DBlob> m_vertexShaderBlob = nullptr;
	List<D3D12_INPUT_ELEMENT_DESC> m_list_inputElementDesc;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	List<ConstantBufferReflection> m_list_constBufferReflection;
	List<TextureReflection> m_list_textureReflection;
	List<SamplerReflection> m_list_samplerReflection;
	ShaderStructReflection m_inputLayoutReflection, m_outputLayoutReflection;
	std::string m_shaderName;
	std::uint32_t m_inputVertexType = 0;
};
class Resource_GeometryShader_Dx12 :public Resource_GeometryShader {
public:
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::stringstream& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_GeometryShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetGeometryShaderBlob();
	const std::string& GetName()const override { return m_shaderName; }
	std::int32_t GetConstantBufferCount() const override { return m_list_constBufferReflection.GetSize(); }
	const List<ConstantBufferReflection>& GetConstantBufferReflectionDatas() const override { return m_list_constBufferReflection; }
	const List<TextureReflection>& GetTextureReflectionDatas() const override { return m_list_textureReflection; }
	const List<SamplerReflection>& GetSamplerReflectionDatas() const override { return m_list_samplerReflection; }
	const ShaderStructReflection& GetInputLayout()const override { return m_inputLayoutReflection; }
	const ShaderStructReflection& GetOutputLayout()const override { return m_outputLayoutReflection; }
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const bool arg_isfxc);
	Microsoft::WRL::ComPtr<ID3DBlob> m_geometryShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> m_vwp_graphicDevice;
	List<ConstantBufferReflection> m_list_constBufferReflection;
	List<TextureReflection> m_list_textureReflection;
	List<SamplerReflection> m_list_samplerReflection;
	ShaderStructReflection m_inputLayoutReflection, m_outputLayoutReflection;
	std::string m_shaderName;
};
}
}
