#include "stdafx.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include <d3dx12.h>
#include "ShaderHelper.h"
void ButiEngine::ShaderHelper::Compile(const std::string& filePath, const CompileType& type)
{
	enum ShaderType{
		vs,ps,gs,undefined
	};

	std::ifstream ifs(filePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	ShaderType shaderType= undefined;


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
		std::cout << filePath << "にはエントリーポイントが見つかりません" << std::endl;
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

	Compile(filePath, source, entryPoint.c_str(), shaderModel.c_str(), type);

	if (shaderType==vs) {
		switch (type)
		{
		case CompileType::Dx11:

			//InputLayoutCompileDx11(source, filePath);
			break;
		case CompileType::Dx12:
			InputLayoutCompileDx12(source, filePath);
			break;
		default:
			break;
		}
	}
	else if (shaderType == ps) {
		OutputLayoutCompileDx12(source, filePath);
	}

}

void ButiEngine::ShaderHelper::Compile(const std::string& filePath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type)
{


	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> buffer = nullptr;
	auto hr = D3DCompile(source.c_str(), source.length(), filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, buffer.GetAddressOf(), error.GetAddressOf());
	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
		return;
	}
	else {
		std::cout << filePath << "のコンパイル成功" << std::endl;
	}
	auto dirPath = StringHelper::GetDirectory(filePath);

	if (dirPath == filePath) {
		dirPath = "Compiled/";
	}
	else {
		dirPath += "Compiled/";
	}

	auto fileName = StringHelper::GetFileName(filePath, false);

	auto dirRes = _mkdir(dirPath.c_str());
	BinaryWriter shaderWriter;

	std::string extension;
	switch (type)
	{
	case CompileType::Dx11:
		extension = ".dx11cps";
		break;
	case CompileType::Dx12:
		extension = ".dx12cps";
		break;
	default:
		break;
	}

	shaderWriter.WriteStart(dirPath + fileName + extension);
	shaderWriter.WriteVariable<std::int32_t>(buffer->GetBufferSize());
	shaderWriter.WriteCharactor((char*)buffer->GetBufferPointer(), buffer->GetBufferSize());
	shaderWriter.WriteEnd();
}
//
//void ButiEngine::ShaderHelper::InputLayoutCompileDx11(const std::string& source, const std::string& filePath)
//{
//	std::vector<D3D11_INPUT_ELEMENT_DESC> vec_inputElementDesc;
//	ShaderHelper::CreateDx11InputElementVector(filePath, source, vec_inputElementDesc);
//
//	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";
//
//	auto fileName = StringHelper::GetFileName(filePath, false);
//	BinaryWriter inputLayoutWriter;
//	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx11ied");
//	inputLayoutWriter.WriteVariable<std::int32_t>(vec_inputElementDesc.size());
//	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {
//		auto write = vec_inputElementDesc.at(i);
//		std::string semantic = write.SemanticName;
//		inputLayoutWriter.WriteVariable<std::int32_t>(semantic.size());
//		inputLayoutWriter.WriteCharactor(semantic);
//		inputLayoutWriter.WriteVariable<DXGI_FORMAT>(write.Format);
//		inputLayoutWriter.WriteVariable<std::uint32_t>(write.InputSlot);
//		inputLayoutWriter.WriteVariable<std::uint32_t>(write.AlignedByteOffset);
//		inputLayoutWriter.WriteVariable<D3D11_INPUT_CLASSIFICATION>(write.InputSlotClass);
//		inputLayoutWriter.WriteVariable<std::uint32_t>(write.InstanceDataStepRate);
//		inputLayoutWriter.WriteVariable<std::uint32_t>(write.SemanticIndex);
//	}
//	inputLayoutWriter.WriteEnd();
//
//	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {
//
//		delete (vec_inputElementDesc.at(i).SemanticName);
//	}
//	vec_inputElementDesc.clear();
//
//}

void ButiEngine::ShaderHelper::InputLayoutCompileDx12(const std::string& source, const std::string& filePath)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	ShaderHelper::CreateDx12InputElementVector(filePath, source, vec_inputElementDesc);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter inputLayoutWriter;
	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx12ied");
	auto descsSize = vec_inputElementDesc.size();
	inputLayoutWriter.WriteVariable<std::int32_t>(descsSize);
	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {
		auto write = vec_inputElementDesc.at(i);
		std::string semantic = write.SemanticName;
		inputLayoutWriter.WriteVariable<std::int32_t>(semantic.size());
		inputLayoutWriter.WriteCharactor(semantic);
		inputLayoutWriter.WriteVariable<DXGI_FORMAT>(write.Format);
		inputLayoutWriter.WriteVariable<std::uint32_t>(write.InputSlot);
		inputLayoutWriter.WriteVariable<std::uint32_t>(write.AlignedByteOffset);
		inputLayoutWriter.WriteVariable<D3D12_INPUT_CLASSIFICATION>(write.InputSlotClass);
		inputLayoutWriter.WriteVariable<std::uint32_t>(write.InstanceDataStepRate);
		inputLayoutWriter.WriteVariable<std::uint32_t>(write.SemanticIndex);
	}
	inputLayoutWriter.WriteEnd();

	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {

		delete (vec_inputElementDesc.at(i).SemanticName);
	}
	vec_inputElementDesc.clear();

}

void ButiEngine::ShaderHelper::OutputLayoutCompileDx12(const std::string& source, const std::string& filePath)
{
	std::vector<DXGI_FORMAT> vec_formats;
	ShaderHelper::CreateDx12PSOutputVector(filePath, source, vec_formats);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter outputLayoutWriter;
	outputLayoutWriter.WriteStart(dirPath + fileName + ".psoutput");
	auto outputsSize = vec_formats.size();
	outputLayoutWriter.WriteVariable<std::int32_t>(outputsSize);
	for (std::int32_t i = 0; i < vec_formats.size(); i++) {
		auto write = vec_formats.at(i);
		outputLayoutWriter.WriteVariable<DXGI_FORMAT>(write);
	}
	outputLayoutWriter.WriteEnd();
	vec_formats.clear();
}


std::vector<ButiEngine::ShaderHelper::InputLayoutElementData> ButiEngine::ShaderHelper::CreateInputLayoutDataVector(const std::string& filePath, const std::string& source)
{
	std::vector<InputLayoutElementData> output;

	std::string inputLayoutStr = source;

	if (StringHelper::Contains(source, "#include")) {


		ShaderFileInclude(filePath, inputLayoutStr);


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

void ButiEngine::ShaderHelper::ShaderFileInclude(const std::string& filePath, std::string& source)
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
//void ButiEngine::ShaderHelper::CreateDx11InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D11_INPUT_ELEMENT_DESC>& arg_vec_elementDesc)
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

void ButiEngine::ShaderHelper::CreateDx12InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D12_INPUT_ELEMENT_DESC>& arg_vec_elementDesc)
{
	auto inputLayoutDatas = CreateInputLayoutDataVector(filePath, source);
	for (auto itr = inputLayoutDatas.begin(); itr != inputLayoutDatas.end(); itr++) {

		char* out = (char*)malloc(itr->SemanticName.size() + 1);
		strncpy_s(out, itr->SemanticName.size() + 1, itr->SemanticName.c_str(), itr->SemanticName.size());

		arg_vec_elementDesc.push_back(D3D12_INPUT_ELEMENT_DESC{ out,0,itr->Format,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	}
}

void ButiEngine::ShaderHelper::CreateDx12PSOutputVector(const std::string& filePath, const std::string& source, std::vector<DXGI_FORMAT>& arg_vec_formats)
{

	std::string PSOutputStr = source;

	if (StringHelper::Contains(source, "#include")) {


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



DXGI_FORMAT ButiEngine::ShaderHelper::GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName)
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
	else if (arg_formatStr == "std::int32_t")
	{
		return  DXGI_FORMAT_R32_SINT;
	}
	else if (arg_formatStr == "min16int")
	{
		return  DXGI_FORMAT_R16_SINT;
	}
	return DXGI_FORMAT_UNKNOWN;
}

