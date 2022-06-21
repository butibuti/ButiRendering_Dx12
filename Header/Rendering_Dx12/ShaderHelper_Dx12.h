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
	BUTIRENDERING_API static void Compile(const std::string& arg_sourceFilePath,const std::string& arg_outputFilePath,const CompileType& type);
	BUTIRENDERING_API static void Compile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type);
	
	BUTIRENDERING_API static void InputLayoutCompile(const std::string& source, const std::string& fileName);
	BUTIRENDERING_API static void OutputLayoutCompileDx12(const std::string& source, const std::string& fileName);
	BUTIRENDERING_API static std::vector<InputLayoutElementData> CreateInputLayoutDataVector(const char*  arg_filePath, const std::string& source);
	BUTIRENDERING_API static void ShaderFileInclude(const std::string& filePath, std::string& source);
	
	BUTIRENDERING_API static void CreateInputElementVector(const char* arg_filePath, const std::string& source, std::vector<InputElement>& arg_vec_elementDesc);
	BUTIRENDERING_API static void CreateDx12PSOutputVector(const char* arg_filePath, const std::string& source, std::vector<std::int32_t>& arg_vec_formats);
	BUTIRENDERING_API static DXGI_FORMAT GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName);

	static void OutputCereal(const std::vector<InputElement>& v, const std::string& arg_path);
	static void InputCereal(std::vector<InputElement>& v, const std::string& arg_path);
	static void OutputCereal(const std::vector<std::int32_t>& v, const std::string& arg_path);
	static void InputCereal(std::vector<std::int32_t>& v, const std::string& arg_path);
};
}
}