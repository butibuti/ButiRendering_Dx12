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
	static void FXCShaderReflection(const void* arg_data, const std::int32_t arg_size, List<ConstantBufferReflection>& arg_output_list_cBuffer, List<TextureReflection>& arg_output_list_texture, List<SamplerReflection>& arg_output_list_sampler, ShaderStructReflection& arg_output_input, ShaderStructReflection& arg_output_output);
	static void DXCShaderReflection(const void* arg_data, const std::int32_t arg_size, List<ConstantBufferReflection>& arg_output_list_cBuffer, List<TextureReflection>& arg_output_list_texture, List<SamplerReflection>& arg_output_list_sampler, ShaderStructReflection& arg_output_input, ShaderStructReflection& arg_output_output);
};
}
}