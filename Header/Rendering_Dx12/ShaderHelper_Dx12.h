#pragma once
#include"stdafx.h"

#include <D3dx12.h>
namespace ButiEngine {
namespace ButiRendering {
class ShaderHelper
{
public:
	struct InputLayoutElementData {
		DXGI_FORMAT Format;
		std::string SemanticName;
	};
	enum class CompileType {
		Dx11 = 0, Dx12 = 1,
	};
	BUTIRENDERING_API static void Compile(const std::string& filePath, const CompileType& type);
	BUTIRENDERING_API static void Compile(const std::string& filePath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type);
	//static void InputLayoutCompileDx11(const std::string& source, const std::string& fileName);
	BUTIRENDERING_API static void InputLayoutCompileDx12(const std::string& source, const std::string& fileName);
	BUTIRENDERING_API static void OutputLayoutCompileDx12(const std::string& source, const std::string& fileName);
	BUTIRENDERING_API static std::vector<InputLayoutElementData> CreateInputLayoutDataVector(const std::string& filePath, const std::string& source);
	BUTIRENDERING_API static void ShaderFileInclude(const std::string& filePath, std::string& source);
	//static void CreateDx11InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D11_INPUT_ELEMENT_DESC>& arg_vec_elementDesc);
	BUTIRENDERING_API static void CreateDx12InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D12_INPUT_ELEMENT_DESC>& arg_vec_elementDesc);
	BUTIRENDERING_API static void CreateDx12PSOutputVector(const std::string& filePath, const std::string& source, std::vector<DXGI_FORMAT>& arg_vec_formats);
	BUTIRENDERING_API static DXGI_FORMAT GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName);
};
}
}