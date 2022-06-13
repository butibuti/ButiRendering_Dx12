#pragma once
#include"stdafx.h"
#include"../Resource_Shader.h"
namespace ButiEngine {

namespace ButiRendering {
class GraphicDevice_Dx12;
namespace Dx12Compile {
static void Compile(const std::string& source, const std::string& fileName, const char* entryPoint, const char* shaderModel);
static void InputLayoutCompile(const std::string& source, const std::string& fileName);

static void ReadCompiled(const std::string& fileName, Microsoft::WRL::ComPtr<ID3DBlob>& out);
static void ReadCompiledInputLayout(const std::string& fileName, std::vector<D3D12_INPUT_ELEMENT_DESC>& vec_inputElementDesc);
static void ReadCompiledPSOutput(const std::string& fileName, std::vector<std::int32_t>& vec_inputElementDesc);
}

class Resource_PixelShader_Dx12 :public Resource_PixelShader
{
public:
	BUTIRENDERING_API Resource_PixelShader_Dx12(const std::string& fileName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_PixelShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;
	BUTIRENDERING_API std::vector<std::int32_t>& GetFormats()override;
	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetPixelShaderBlob();
	BUTIRENDERING_API std::string GetName()const override;
	BUTIRENDERING_API std::vector< std::uint32_t> GetOutputPixelFormat()const override;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::vector<std::int32_t> vec_formats;
	std::string shaderName;
	std::uint32_t outputPixelFormat;
};
class Resource_VertexShader_Dx12 :public Resource_VertexShader
{
public:
	BUTIRENDERING_API Resource_VertexShader_Dx12(const std::string& fileName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_VertexShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetVertexShaderBlob();
	BUTIRENDERING_API std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayoutVector();
	BUTIRENDERING_API std::string GetName()const override;
	BUTIRENDERING_API std::uint32_t GetInputVertexType()const override;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::string shaderName;
	std::uint32_t inputVertexType = 0;
};
class Resource_GeometryShader_Dx12 :public Resource_GeometryShader {
public:
	BUTIRENDERING_API Resource_GeometryShader_Dx12(const std::string& fileName, Value_ptr<GraphicDevice> arg_graphicDevice);
	BUTIRENDERING_API ~Resource_GeometryShader_Dx12();
	BUTIRENDERING_API void Attach()const override;
	BUTIRENDERING_API void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

	BUTIRENDERING_API Microsoft::WRL::ComPtr<ID3DBlob>& GetGeometryShaderBlob();
	BUTIRENDERING_API std::string GetName()const override;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> geometryShaderBlob = nullptr;
	Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	std::string shaderName;
};
}
}