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

	if (StringHelper::Contains(source, "VSMain")) {
		shaderType = vs;
		entryPoint = "VSMain";
		shaderModel = "vs_";
	}
	else if (StringHelper::Contains(source, "PSMain")) {
		shaderType = ps;
		entryPoint = "PSMain";
		shaderModel = "ps_";
	}
	else if (StringHelper::Contains(source, "GSMain")) {
		shaderType = gs;
		entryPoint = "GSMain";
		shaderModel = "gs_";
	}
	else {
		std::cout << arg_sourceFilePath << "�ɂ̓G���g���[�|�C���g��������܂���" << std::endl;
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

	Compile(arg_sourceFilePath, arg_outputFilePath, source, entryPoint.c_str(), shaderModel.c_str(), type);
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
		std::cout << arg_sourceFilePath << "�̃R���p�C������" << std::endl;
	}
	auto dirRes = _mkdir(StringHelper::GetDirectory(arg_outputFilePath).c_str());
	BinaryWriter shaderWriter;
	shaderWriter.WriteStart(arg_outputFilePath);
	shaderWriter.WriteCharactor(static_cast<char*>(buffer->GetBufferPointer()), buffer->GetBufferSize());
	shaderWriter.WriteEnd();
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
		else if (arg_formatStr == "int4")
		{

			return  Format::R32G32B32A32_SINT;
		}
		else if (arg_formatStr == "int3")
		{
			return  Format::R32G32B32_SINT;
		}
		else if (arg_formatStr == "int2")
		{
			return  Format::R32G32_SINT;
		}
		else if (arg_formatStr == "int")
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
	D3DReflect(arg_data, arg_size, IID_PPV_ARGS(&shaderReflection));
	D3D12_SHADER_DESC desc{};
	shaderReflection->GetDesc(&desc);

	for (std::int32_t index = 0; index < desc.ConstantBuffers; index++) {
		D3D12_SHADER_BUFFER_DESC shaderBufDesc{};
		auto cbuffer = shaderReflection->GetConstantBufferByIndex(index);
		cbuffer->GetDesc(&shaderBufDesc);
		ConstantBufferReflection cbufferRefl;
		cbufferRefl.m_outputLayoutName = shaderBufDesc.Name;
		for (auto j = 0; j < shaderBufDesc.Variables; ++j)
		{
			D3D12_SHADER_VARIABLE_DESC varDesc{};
			D3D12_SHADER_TYPE_DESC typeDesc;
			Microsoft::WRL::ComPtr<ID3D12ShaderReflectionVariable> varRefl = cbuffer->GetVariableByIndex(j);
			Microsoft::WRL::ComPtr<ID3D12ShaderReflectionType> varTypeRefl = varRefl->GetType();

			varRefl->GetDesc(&varDesc);
			varTypeRefl->GetDesc(&typeDesc);

			cbufferRefl.m_list_format.Add(ShaderTypeToFormat( typeDesc.Name,varDesc.Name));
		}
		arg_output_list_cBuffer.Add(cbufferRefl);
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
		elem.format = ShaderTypeToFormat(formatName, "");
		elem.semanticName = paramDesc.SemanticName;
		elem.semanticIndex = paramDesc.SemanticIndex;
		elem.inputSlot= paramDesc.Register;

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
		elem.format = ShaderTypeToFormat(formatName, "");
		elem.semanticName = paramDesc.SemanticName;
		elem.semanticIndex = paramDesc.SemanticIndex;
		elem.inputSlot = paramDesc.Register;

		arg_output_input.m_list_element.Add(elem);
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

	//// �V�F�[�_�[�o�C�i���f�[�^�����[�h���ADXIL�`�����N�u���b�N�i�̃C���f�b�N�X�j�𓾂�.
	//UINT shdIndex = 0;
	//refl->Load(binBlob.Get());
	//refl->FindFirstPartKind(DXIL_FOURCC('D', 'X', 'I', 'L'), &shdIndex);
}
void ButiEngine::ButiRendering::ShaderHelper::OutputCereal(const std::vector<ShaderElement>& v, const std::string& arg_path)
{
	std::stringstream stream;
	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);
	std::ofstream outputFile(arg_path);
	outputFile << stream.str();
	outputFile.close();
	stream.clear();
}

void ButiEngine::ButiRendering::ShaderHelper::InputCereal(std::vector<ShaderElement>& v, const std::string& arg_path)
{
	std::stringstream stream;
	std::ifstream inputFile(arg_path, std::ios::binary);
	stream << inputFile.rdbuf();
	cereal::BinaryInputArchive binInputArchive(stream);
	binInputArchive(v);
}

void ButiEngine::ButiRendering::ShaderHelper::OutputCereal(const std::vector<std::int32_t>& v, const std::string& arg_path)
{
	std::stringstream stream;
	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);
	std::ofstream outputFile(arg_path);
	outputFile << stream.str();
	outputFile.close();
	stream.clear();
}

void ButiEngine::ButiRendering::ShaderHelper::InputCereal(std::vector<std::int32_t>& v, const std::string& arg_path)
{
	std::stringstream stream;
	std::ifstream inputFile(arg_path, std::ios::binary);
	stream << inputFile.rdbuf();
	cereal::BinaryInputArchive binInputArchive(stream);
	binInputArchive(v);
}
