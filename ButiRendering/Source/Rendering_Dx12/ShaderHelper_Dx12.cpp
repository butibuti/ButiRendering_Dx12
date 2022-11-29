#include "stdafx.h"
#include <dxcapi.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include <d3dx12.h>
#include "ButiRendering_Dx12/Header/Rendering_Dx12/ShaderHelper_Dx12.h"
#include "ButiRendering_Dx12/Header/ShaderReflection.h"
#include<direct.h>
#include"ButiEngineHeader/Header/Common/CerealUtil.h"
void ButiEngine::ButiRendering::ShaderHelper::Compile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath, const CompileType& type)
{
	enum ShaderType {
		vs, ps, gs, undefined
	};

	std::ifstream ifs(arg_sourceFilePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	ShaderType shaderType = ShaderType::undefined;


	std::string entryPoint;
	std::string shaderModel;
	std::string outputFilePath = arg_outputFilePath;
	if (StringHelper::Contains(source, "VSMain")) {
		shaderType = vs;
		entryPoint = "VSMain";
		shaderModel = "vs_";
		outputFilePath += StringHelper::Contains(outputFilePath, '.') ? "" : ".cvs";
	}
	else if (StringHelper::Contains(source, "PSMain")) {
		shaderType = ps;
		entryPoint = "PSMain";
		shaderModel = "ps_";
		outputFilePath += StringHelper::Contains(outputFilePath, '.') ? "" : ".cps";
	}
	else if (StringHelper::Contains(source, "GSMain")) {
		shaderType = gs;
		entryPoint = "GSMain";
		shaderModel = "gs_";
		outputFilePath += StringHelper::Contains(outputFilePath, '.') ? "" : ".cgs";
	}
	else {
		std::cout << arg_sourceFilePath << "にはエントリーポイントが見つかりません" << std::endl;
		return;
	}


	switch (type)
	{
	case CompileType::Dx11:


		shaderModel += "4_0";
		break;
	case CompileType::Dx12:

		shaderModel += "5_1";
		break;
	default:
		break;
	}

	Compile(arg_sourceFilePath, outputFilePath, source, entryPoint.c_str(), shaderModel.c_str(), type);
}

void ButiEngine::ButiRendering::ShaderHelper::Compile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> buffer = nullptr;
	auto hr = D3DCompile(source.c_str(), source.length(), arg_sourceFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, buffer.GetAddressOf(), error.GetAddressOf());
	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
		return;
	}
	else {
		std::cout << arg_sourceFilePath << "のコンパイル成功" << std::endl;
	}
	auto dirRes = _mkdir(StringHelper::GetDirectory(arg_outputFilePath).c_str());
	BinaryWriter_File shaderWriter(arg_outputFilePath);
	shaderWriter.WriteCharactor(static_cast<char*>(buffer->GetBufferPointer()), buffer->GetBufferSize());
}
namespace ButiEngine {
namespace ButiRendering {
inline Format ShaderTypeToFormat(const std::string& arg_formatStr, const std::string& arg_semanticName)
{
	if (arg_semanticName == "POSITION") {
		return Format::R32G32B32_FLOAT;
	}
	if (arg_formatStr == "float4") {
		return  Format::R32G32B32A32_FLOAT;
	}
	else
		if (arg_formatStr == "OutputColor") {
			return  Format::R8G8B8A8_UNORM;
		}
		else if (arg_formatStr == "float3")
		{
			return  Format::R32G32B32_FLOAT;
		}
		else if (arg_formatStr == "float2")
		{
			return  Format::R32G32_FLOAT;
		}
		else if (arg_formatStr == "float")
		{
			return  Format::R32_FLOAT;
		}
		else if (arg_formatStr == "uint4")
		{

			return  Format::R32G32B32A32_UINT;
		}
		else if (arg_formatStr == "uint3")
		{

			return  Format::R32G32B32_UINT;
		}
		else if (arg_formatStr == "uint2")
		{

			return  Format::R32G32_UINT;
		}
		else if (arg_formatStr == "uint")
		{

			return  Format::R32_UINT;
		}
		else if (arg_formatStr == "min16uint")
		{
			return  Format::R16_UINT;
		}
		else if (arg_formatStr == "sint4")
		{

			return  Format::R32G32B32A32_SINT;
		}
		else if (arg_formatStr == "sint3")
		{
			return  Format::R32G32B32_SINT;
		}
		else if (arg_formatStr == "sint2")
		{
			return  Format::R32G32_SINT;
		}
		else if (arg_formatStr == "sint")
		{
			return  Format::R32_SINT;
		}
		else if (arg_formatStr == "min16int")
		{
			return  Format::R16_SINT;
		}
	return Format::UNKNOWN;
}

}
}

void ButiEngine::ButiRendering::ShaderHelper::FXCShaderReflection(const void* arg_data, const std::int32_t arg_size, List<ConstantBufferReflection>& arg_output_list_cBuffer, List<TextureReflection>& arg_output_list_texture, List<SamplerReflection>& arg_output_list_sampler, ShaderStructReflection& arg_output_input, ShaderStructReflection& arg_output_output) {

	Microsoft::WRL::ComPtr<ID3D12ShaderReflection> shaderReflection;
	D3DReflect(arg_data, arg_size, IID_ID3D12ShaderReflection, &shaderReflection);
	List<D3D12_SHADER_INPUT_BIND_DESC> list_bindData;
	D3D12_SHADER_DESC desc{};
	shaderReflection->GetDesc(&desc);
	for (std::int32_t index = 0; index < desc.BoundResources; index++) {
		D3D12_SHADER_INPUT_BIND_DESC desc{};
		shaderReflection->GetResourceBindingDesc(index, &desc);
		list_bindData.Add(desc);
	}
	for (std::int32_t index = 0; index < desc.ConstantBuffers; index++) {
		D3D12_SHADER_BUFFER_DESC shaderBufDesc{};
		auto cbuffer = shaderReflection->GetConstantBufferByIndex(index);
		cbuffer->GetDesc(&shaderBufDesc);

		ConstantBufferReflection cbufferRefl;
		cbufferRefl.m_bufferName = shaderBufDesc.Name;
		for (auto j = 0; j < shaderBufDesc.Variables; ++j)
		{
			D3D12_SHADER_VARIABLE_DESC varDesc{};D3D12_SHADER_TYPE_DESC typeDesc;
			auto varRefl = cbuffer->GetVariableByIndex(j);
			auto varTypeRefl = varRefl->GetType();

			varRefl->GetDesc(&varDesc);
			varTypeRefl->GetDesc(&typeDesc);

			cbufferRefl.m_list_format.Add(ShaderTypeToFormat( typeDesc.Name,varDesc.Name));
		}
		std::function<bool(const D3D12_SHADER_INPUT_BIND_DESC& check)> findFunc = [cbufferRefl](const D3D12_SHADER_INPUT_BIND_DESC& check)->bool {return check.Name == cbufferRefl.m_bufferName; };
		cbufferRefl.m_slot= list_bindData.Find(findFunc)->BindPoint;
		arg_output_list_cBuffer.Add(cbufferRefl);
	}
	for (auto& bindDesc : list_bindData) {
		if (bindDesc.Type == D3D_SIT_TEXTURE) {
			arg_output_list_texture.Add({bindDesc.Name});
		}
		else if(bindDesc.Type == D3D_SIT_SAMPLER) {
			arg_output_list_sampler.Add({ bindDesc.Name });
		}
	}

	for (std::int32_t index = 0; index < desc.InputParameters; index++) {
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc{};
		shaderReflection->GetInputParameterDesc(index, &paramDesc);
		std::string formatName = "";
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			formatName = "uint";
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			formatName = "sint";
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			formatName = "float";
			break;
		default:
			formatName = "Unknown";
			break;
		}

		BYTE elementCount = paramDesc.Mask;
		elementCount = ((elementCount & 0xA) >> 1) + ((elementCount & 0x5));
		elementCount = ((elementCount & 0xC) >> 2) + ((elementCount & 0x3));
		formatName += elementCount > 1 ? std::to_string(static_cast<std::uint32_t>(elementCount)) : "";

		ShaderElement elem;
		elem.format = ShaderTypeToFormat(formatName, paramDesc.SemanticName);
		elem.semanticName = paramDesc.SemanticName;
		elem.semanticIndex = paramDesc.SemanticIndex;

		arg_output_input.m_list_element.Add(elem);
	}
	for (std::int32_t index = 0; index < desc.OutputParameters; index++) {
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc{};
		shaderReflection->GetOutputParameterDesc(index, &paramDesc);
		std::string formatName = "";
		switch (paramDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			formatName = "uint";
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			formatName = "sint";
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			formatName = "float";
			break;
		default:
			formatName = "Unknown";
			break;
		}

		BYTE elementCount = paramDesc.Mask;
		elementCount = ((elementCount & 0xA) >> 1) + ((elementCount & 0x5));
		elementCount = ((elementCount & 0xC) >> 2) + ((elementCount & 0x3));
		formatName += elementCount > 1 ? std::to_string(static_cast<std::uint32_t>(elementCount)) : "";

		ShaderElement elem;
		elem.format = ShaderTypeToFormat(formatName, paramDesc.SemanticName);
		elem.semanticName = paramDesc.SemanticName;
		elem.semanticIndex = paramDesc.SemanticIndex;

		arg_output_output.m_list_element.Add(elem);
	}
}

void ButiEngine::ButiRendering::ShaderHelper::DXCShaderReflection(const void* arg_data, const std::int32_t arg_size, List<ConstantBufferReflection>& arg_output_list_cBuffer, List<TextureReflection>& arg_output_list_texture, List<SamplerReflection>& arg_output_list_sampler, ShaderStructReflection& arg_output_input, ShaderStructReflection& arg_output_output)
{
	assert(0);
	//Microsoft::WRL::ComPtr<IDxcLibrary> lib;
	//DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&lib));

	//Microsoft::WRL::ComPtr<IDxcBlobEncoding> binBlob{};
	//lib->CreateBlobWithEncodingOnHeapCopy(data.data(), data.size(), CP_ACP, &binBlob);

	//Microsoft::WRL::ComPtr<IDxcContainerReflection> refl;
	//DxcCreateInstance(CLSID_DxcContainerReflection, IID_PPV_ARGS(&refl));

	//// シェーダーバイナリデータをロードし、DXILチャンクブロック（のインデックス）を得る.
	//UINT shdIndex = 0;
	//refl->Load(binBlob.Get());
	//refl->FindFirstPartKind(DXIL_FOURCC('D', 'X', 'I', 'L'), &shdIndex);
}