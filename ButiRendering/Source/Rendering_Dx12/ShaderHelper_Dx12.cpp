#include "stdafx.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include <d3dx12.h>
#include "ButiRendering_Dx12/Header/Rendering_Dx12/ShaderHelper_Dx12.h"
#include<direct.h>
#include"ButiEngineHeader/Header/Common/CerealUtil.h"
void ButiEngine::ButiRendering::ShaderHelper::Compile(const std::string& arg_sourceFilePath, const std::string& arg_outputFilePath, const CompileType& type)
{
	enum ShaderType{
		vs,ps,gs,undefined
	};

	std::ifstream ifs(arg_sourceFilePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	ShaderType shaderType=ShaderType::undefined;


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

	Compile(arg_sourceFilePath, arg_outputFilePath,source, entryPoint.c_str(), shaderModel.c_str(), type);

	if (shaderType==vs) {
		switch (type)
		{
		case CompileType::Dx11:

			//InputLayoutCompileDx11(source, filePath);
			break;
		case CompileType::Dx12:
			InputLayoutCompile(source, arg_outputFilePath);
			break;
		default:
			break;
		}
	}
	else if (shaderType == ps) {
		OutputLayoutCompileDx12(source, arg_outputFilePath);
	}

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
	BinaryWriter shaderWriter;
	shaderWriter.WriteStart(arg_outputFilePath);
	shaderWriter.WriteCharactor(static_cast<char*>(buffer->GetBufferPointer()), buffer->GetBufferSize());
	shaderWriter.WriteEnd();
}

void ButiEngine::ButiRendering::ShaderHelper::InputLayoutCompile(const std::string& source, const std::string& filePath)
{
	std::vector<InputElement> vec_inputElementDesc;
	ShaderHelper::CreateInputElementVector(filePath.c_str(), source, vec_inputElementDesc);
	auto fileName = StringHelper::RemoveExtension(filePath);
	OutputCereal(vec_inputElementDesc, fileName + ".inputLayout");

}

void ButiEngine::ButiRendering::ShaderHelper::OutputLayoutCompileDx12(const std::string& source, const std::string& filePath)
{
	std::vector<std::int32_t> vec_formats;
	ShaderHelper::CreateDx12PSOutputVector(filePath.c_str(), source, vec_formats);
	auto fileName = StringHelper::RemoveExtension(filePath);
	OutputCereal(vec_formats, fileName + ".psoutput");
}


std::vector<ButiEngine::ButiRendering::ShaderHelper::InputLayoutElementData> ButiEngine::ButiRendering::ShaderHelper::CreateInputLayoutDataVector(const char* arg_filePath, const std::string& source)
{
	std::vector<InputLayoutElementData> output;

	std::string inputLayoutStr = source;

	if (StringHelper::Contains(source, "#include")) {
		ShaderFileInclude(arg_filePath, inputLayoutStr);
	}
	std::string vsMain = StringHelper::Cut(inputLayoutStr, "VSMain(", ")", false);
	auto args = StringHelper::Split(vsMain, " ");
	std::string argmentType;
	for (auto itr = args.begin(); itr != args.end(); itr++) {
		if (itr->length() > 2) {
			argmentType = *itr;
			break;
		}
	}

	inputLayoutStr = StringHelper::Cut(inputLayoutStr, "struct " + argmentType + "\n{\n", "\n};", false);


	inputLayoutStr = StringHelper::RemoveComment(inputLayoutStr);

	inputLayoutStr = StringHelper::Remove(inputLayoutStr, "\t");
	inputLayoutStr = StringHelper::Remove(inputLayoutStr, "\n");

	auto vec_layout = StringHelper::Split(inputLayoutStr, ";");


	for (auto itr = vec_layout.begin(); itr != vec_layout.end(); itr++) {
		auto spaceSplitedSource = StringHelper::Split(*itr, " ");
		auto format = spaceSplitedSource.begin();
		while (format->size() < 2)
		{
			format++;
		}
		std::string semanticName = StringHelper::Remove(*(StringHelper::Split(*itr, ":").end() - 1), " ");
		output.push_back({ GetFormat(*format,semanticName),semanticName });
	}
	return output;
}

void ButiEngine::ButiRendering::ShaderHelper::ShaderFileInclude(const std::string& filePath, std::string& source)
{
	while (StringHelper::Contains(source, "#include")) {
		std::string includeFileName = StringHelper::Cut(source, "#include\"", "\"", false);
		source = StringHelper::Remove(source, "#include\"" + includeFileName + "\"");
		auto path = StringHelper::GetDirectory(filePath);
		std::ifstream includeFile(path + includeFileName);
		source = std::string(std::istreambuf_iterator<char>(includeFile),
			std::istreambuf_iterator<char>()) + source;
	}
}
//
//void ButiEngine::ButiRendering::ShaderHelper::CreateDx11InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D11_INPUT_ELEMENT_DESC>& arg_vec_elementDesc)
//{
//
//	auto inputLayoutDatas = CreateInputLayoutDataVector(filePath, source);
//	for (auto itr = inputLayoutDatas.begin(); itr != inputLayoutDatas.end(); itr++) {
//		char* out = (char*)malloc(itr->SemanticName.size() + 1);
//		strncpy_s(out, itr->SemanticName.size() + 1, itr->SemanticName.c_str(), itr->SemanticName.size());
//		arg_vec_elementDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ out,0,itr->Format,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
//
//	}
//}

void ButiEngine::ButiRendering::ShaderHelper::CreateInputElementVector(const char* arg_filePath, const std::string& source, std::vector<InputElement>& arg_vec_elementDesc)
{
	auto inputLayoutDatas = CreateInputLayoutDataVector(arg_filePath, source);
	InputElement elem;
	elem.inputSlot = 0;
	elem.instanceDataStepRate = 0;
	elem.semanticIndex = 0;
	elem.alignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elem.inputSlotClass = InputClassfication::PER_VERTEX_DATA;
	for (auto itr = inputLayoutDatas.begin(); itr != inputLayoutDatas.end(); itr++) {
		elem.semanticName = itr->SemanticName;
		elem.format =static_cast<Format>( itr->Format);
		arg_vec_elementDesc.push_back(elem);
	}
}

void ButiEngine::ButiRendering::ShaderHelper::CreateDx12PSOutputVector(const char* arg_filePath, const std::string& source, std::vector<std::int32_t>& arg_vec_formats)
{

	std::string PSOutputStr = source;

	if (StringHelper::Contains(source, "#include")) {
		std::string filePath = arg_filePath ? std::string(arg_filePath) : std::string();
		ShaderFileInclude(filePath, PSOutputStr);
	}
	std::string psMainLine = StringHelper::BackIdentifer(PSOutputStr, "PSMain");
	auto returns = StringHelper::Split(psMainLine, " ");
	std::string returnType;
	for (auto itr = returns.begin(); itr != returns.end(); itr++) {
		if (itr->length() > 2) {
			returnType = *itr;
			break;
		}
	}
	if (returnType == "float4") {
		arg_vec_formats.push_back(DXGI_FORMAT_R8G8B8A8_UNORM);
		return;
	}
	else if (returnType == "float2") {
		arg_vec_formats.push_back(DXGI_FORMAT_R32G32_FLOAT);
		return;
	}

	PSOutputStr = StringHelper::Cut(PSOutputStr, "struct " + returnType + "\n{\n", "\n};", false);


	PSOutputStr = StringHelper::RemoveComment(PSOutputStr);

	PSOutputStr = StringHelper::Remove(PSOutputStr, "\t");
	PSOutputStr = StringHelper::Remove(PSOutputStr, "\n");

	auto vec_layout = StringHelper::Split(PSOutputStr, ";");

	for (auto itr = vec_layout.begin(); itr != vec_layout.end(); itr++) {
		auto spaceSplitedSource = StringHelper::Split(*itr, " ");
		auto format = spaceSplitedSource.begin();
		for (;format != spaceSplitedSource.end();format++)
		{
			if (format->size() > 2) {
				arg_vec_formats.push_back(GetFormat(*format, "noSemantics"));
				break;
			}
		}

	}

}



DXGI_FORMAT ButiEngine::ButiRendering::ShaderHelper::GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName)
{
	if (arg_semanticName == "POSITION") {
		return DXGI_FORMAT_R32G32B32_FLOAT;
	}
	if (arg_formatStr == "float4") {
		return  DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	else
		if (arg_formatStr == "OutputColor") {
			return  DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if (arg_formatStr == "float3")
	{
		return  DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (arg_formatStr == "float2")
	{
		return  DXGI_FORMAT_R32G32_FLOAT;
	}
	else if (arg_formatStr == "float")
	{
		return  DXGI_FORMAT_R32_FLOAT;
	}
	else if (arg_formatStr == "uint4")
	{

		return  DXGI_FORMAT_R32G32B32A32_UINT;
	}
	else if (arg_formatStr == "uint3")
	{

		return  DXGI_FORMAT_R32G32B32_UINT;
	}
	else if (arg_formatStr == "uint2")
	{

		return  DXGI_FORMAT_R32G32_UINT;
	}
	else if (arg_formatStr == "uint")
	{

		return  DXGI_FORMAT_R32_UINT;
	}
	else if (arg_formatStr == "min16uint")
	{
		return  DXGI_FORMAT_R16_UINT;
	}
	else if (arg_formatStr == "int4")
	{

		return  DXGI_FORMAT_R32G32B32A32_SINT;
	}
	else if (arg_formatStr == "int3")
	{
		return  DXGI_FORMAT_R32G32B32_SINT;
	}
	else if (arg_formatStr == "int2")
	{
		return  DXGI_FORMAT_R32G32_SINT;
	}
	else if (arg_formatStr == "int")
	{
		return  DXGI_FORMAT_R32_SINT;
	}
	else if (arg_formatStr == "min16int")
	{
		return  DXGI_FORMAT_R16_SINT;
	}
	return DXGI_FORMAT_UNKNOWN;
}

void ButiEngine::ButiRendering::ShaderHelper::OutputCereal(const std::vector<InputElement>& v, const std::string& arg_path)
{
	std::stringstream stream;
	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);
	std::ofstream outputFile(arg_path);
	outputFile << stream.str();
	outputFile.close();
	stream.clear();
}

void ButiEngine::ButiRendering::ShaderHelper::InputCereal(std::vector<InputElement>& v, const std::string& arg_path)
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

