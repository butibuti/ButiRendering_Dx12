#pragma once
#include"stdafx.h"
#include"../Resource_Shader.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
namespace ShaderCompile_Dx12 {
static void Compile(const std::string& arg_source, const char* arg_filePath, Microsoft::WRL::ComPtr<ID3DBlob>& arg_out, const char* entryPoint, const char* shaderModel);
}

class Resource_PixelShader_Dx12 :public Resource_PixelShader
{
public:
	BUTIRENDERING_API Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath,const std::string& arg_outputFormatSource ,Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_PixelShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_PixelShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName,const std::vector<std::int32_t>& arg_vec_formats, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_PixelShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;
	BUTIRENDERING_API std::vector<std::int32_t>& GetFormats()override;
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetPixelShaderBlob();
	BUTIRENDERING_API std::string GetName()const override;
	BUTIRENDERING_API std::vector< std::uint32_t> GetOutputPixelFormat()const override;
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::vector<std::int32_t>& arg_vec_format);
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::vector<std::int32_t> vec_formats;
	std::string shaderName;
	std::uint32_t outputPixelFormat;
};
class Resource_VertexShader_Dx12 :public Resource_VertexShader
{
public:
	BUTIRENDERING_API Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_shaderPath, const std::string& arg_inputLayoutPath, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_VertexShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_VertexShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName,const std::vector<InputElement>& arg_vec_inputElemnt,Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_VertexShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetVertexShaderBlob();
	BUTIRENDERING_API std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayoutVector();
	BUTIRENDERING_API std::string GetName()const override;
	BUTIRENDERING_API std::uint32_t GetInputVertexType()const override;
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size,const std::vector<InputElement>& arg_vec_inputElemnt);
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::string shaderName;
	std::uint32_t inputVertexType = 0;
};
class Resource_GeometryShader_Dx12 :public Resource_GeometryShader {
public:
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const std::string& arg_shaderPath, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const std::string& arg_shaderName, const std::string& arg_source, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const unsigned char* arg_compiledSource, const std::int32_t arg_size, const std::string& arg_shaderName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_GeometryShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetGeometryShaderBlob();
	BUTIRENDERING_API std::string GetName()const override;
private:
	void CreateShaderData(const unsigned char* arg_compiledSource, const std::int32_t arg_size);
	Microsoft::WRL::ComPtr<ID3DBlob> geometryShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::string shaderName;
};
}
}
