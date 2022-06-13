#include"stdafx.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/ShaderHelper_Dx12.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Shader_Dx12.h"
#include"ButiRendering_Dx12/Header/Vertex.h"
#include<direct.h>
ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12( const std::string& fileName, Value_ptr<GraphicDevice> arg_graphicDevice)
	
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	shaderName = StringHelper::GetFileName(fileName,false);
	Dx12Compile::ReadCompiled(fileName,pixelShaderBlob);
	Dx12Compile::ReadCompiledPSOutput(fileName, vec_formats);
}


ButiEngine::ButiRendering::Resource_PixelShader_Dx12::~Resource_PixelShader_Dx12()
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::Attach() const
{

}

void ButiEngine::ButiRendering::Resource_PixelShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{

	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

std::vector<std::int32_t>& ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetFormats()
{
	return vec_formats;
}


Microsoft::WRL::ComPtr<ID3D10Blob>& ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetPixelShaderBlob()
{
	return pixelShaderBlob;
}

std::string ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetName() const
{
	return shaderName;
}

std::vector<std::uint32_t> ButiEngine::ButiRendering::Resource_PixelShader_Dx12::GetOutputPixelFormat() const
{
	assert(0 && "ToDo");
	return std::vector<std::uint32_t>();
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12( const std::string& filePath, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();

	shaderName = StringHelper::GetFileName(filePath, false);
	{
		//Dx12Compile::Compile(source, filePath, "VSMain", "vs_5_1");
		
		Dx12Compile::ReadCompiled(filePath,vertexShaderBlob);
		Dx12Compile::ReadCompiledInputLayout(filePath, vec_inputElementDesc);
		for (auto inputLayout : vec_inputElementDesc) {
			auto sementicStr = std::string(inputLayout.SemanticName);
			if (sementicStr == "NORMAL") {
				inputVertexType |= Vertex::VertexFlag::Normal;
			}
			else if (sementicStr == "TEXCOORD") {
				inputVertexType |= Vertex::VertexFlag::UV;
			}
			else if (sementicStr == "TANGENT") {
				inputVertexType |= Vertex::VertexFlag::Tangent;
			}
			else if (sementicStr == "COLOR") {
				inputVertexType |= Vertex::VertexFlag::Color;
			}
			else if (sementicStr == "BONE") {
				inputVertexType |= Vertex::VertexFlag::SingleBone;
			}
			else if (sementicStr == "BONEINDEXTWO") {
				inputVertexType |= Vertex::VertexFlag::DouleBone;
			}
			else if (sementicStr == "BONEINDEXFOUR") {
				inputVertexType ^= Vertex::VertexFlag::DouleBone;
				inputVertexType |= Vertex::VertexFlag::QuadBone;
			}
			else if (sementicStr == "SDEFC") {
				inputVertexType ^= Vertex::VertexFlag::QuadBone;
				inputVertexType |= Vertex::VertexFlag::PMX;
			}
		}
	}
}

ButiEngine::ButiRendering::Resource_VertexShader_Dx12::~Resource_VertexShader_Dx12()
{
	for (std::int32_t i = 0; i < vec_inputElementDesc.size(); i++) {
		delete (vec_inputElementDesc.at(i).SemanticName);
	}
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_VertexShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetVertexShaderBlob()
{
	return vertexShaderBlob;
}

std::vector<D3D12_INPUT_ELEMENT_DESC>& ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputLayoutVector()
{
	return vec_inputElementDesc;
}

std::string ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetName() const
{
	return shaderName;
}

std::uint32_t ButiEngine::ButiRendering::Resource_VertexShader_Dx12::GetInputVertexType() const
{
	return inputVertexType;
}

void ButiEngine::ButiRendering::Dx12Compile::Compile(const std::string& source, const std::string& filePath, const char* entryPoint, const char* shaderModel)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> buffer = nullptr;
	auto hr = D3DCompile(source.c_str(), source.length(), filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0,buffer.GetAddressOf(), error.GetAddressOf());

	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
	}
	auto dirPath =StringHelper::GetDirectory( filePath)+"Compiled/";
	
	auto fileName = StringHelper::GetFileName(filePath,false);

	auto dirRes = _mkdir(dirPath.c_str());
	BinaryWriter shaderWriter;
	shaderWriter.WriteStart(dirPath+fileName + ".dx12cps");
	shaderWriter.WriteVariable<std::int32_t>(buffer->GetBufferSize());
	shaderWriter.WriteCharactor((char*)buffer->GetBufferPointer(),buffer->GetBufferSize());
	shaderWriter.WriteEnd();


}

void ButiEngine::ButiRendering::Dx12Compile::InputLayoutCompile(const std::string& source, const std::string& filePath)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	ShaderHelper::CreateDx12InputElementVector(filePath, source, vec_inputElementDesc);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter inputLayoutWriter;
	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx12ied");
	inputLayoutWriter.WriteVariable<std::int32_t>(vec_inputElementDesc.size());
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

void ButiEngine::ButiRendering::Dx12Compile::ReadCompiled(const std::string& filePath,  Microsoft::WRL::ComPtr<ID3DBlob>& out)
{


	auto dirPath = StringHelper::GetDirectory(filePath) ;

	auto fileName = StringHelper::GetFileName(filePath, false);


	BinaryReader shaderReader;
	shaderReader.ReadStart(dirPath + fileName + ".dx12cps");

	std::int32_t size = shaderReader.ReadVariable<std::int32_t>();

	auto hr = D3DCreateBlob(size, out.GetAddressOf());
	auto compiledData = shaderReader.ReadCharactor();

	memcpy(out->GetBufferPointer(), compiledData,size);
	shaderReader.ReadEnd();

	delete compiledData;
}

void ButiEngine::ButiRendering::Dx12Compile::ReadCompiledInputLayout(const std::string& filePath, std::vector<D3D12_INPUT_ELEMENT_DESC>& vec_inputElementDesc)
{
	auto dirPath = StringHelper::GetDirectory(filePath);
	BinaryReader inputLayoutReader;

	auto fileName = StringHelper::GetFileName(filePath.c_str(),false);

	inputLayoutReader.ReadStart(dirPath + fileName + ".dx12ied");
	auto size = inputLayoutReader.ReadVariable<std::int32_t>();
	vec_inputElementDesc.reserve(size);
	for (std::int32_t i = 0; i < size; i++) {
		auto read = D3D12_INPUT_ELEMENT_DESC();
		auto semanticSize = inputLayoutReader.ReadVariable<std::int32_t>();
		auto semanticName = inputLayoutReader.ReadCharactor(semanticSize);
		char* semanticP = (char*)malloc(semanticName.size() + 1);
		memcpy(semanticP, semanticName.c_str(), semanticName.size() + 1);
		read.SemanticName = semanticP;
		read.Format = inputLayoutReader.ReadVariable<DXGI_FORMAT>();
		read.InputSlot = inputLayoutReader.ReadVariable<std::uint32_t>();
		read.AlignedByteOffset = inputLayoutReader.ReadVariable<std::uint32_t>();
		read.InputSlotClass = inputLayoutReader.ReadVariable<D3D12_INPUT_CLASSIFICATION>();
		read.InstanceDataStepRate = inputLayoutReader.ReadVariable<std::uint32_t>();
		read.SemanticIndex = inputLayoutReader.ReadVariable<std::uint32_t>();

		vec_inputElementDesc.push_back(read);
	}

	inputLayoutReader.ReadEnd();
}

void ButiEngine::ButiRendering::Dx12Compile::ReadCompiledPSOutput(const std::string& filePath, std::vector<std::int32_t>& vec_psOutputs) {
	auto dirPath = StringHelper::GetDirectory(filePath);
	BinaryReader inputLayoutReader;

	auto fileName = StringHelper::GetFileName(filePath.c_str(), false);

	if (!Util::ExistFile(dirPath + fileName + ".psoutput")) {
		vec_psOutputs.push_back(DXGI_FORMAT_R8G8B8A8_UNORM);
		return;
	}


	inputLayoutReader.ReadStart(dirPath + fileName + ".psoutput");
	auto size = inputLayoutReader.ReadVariable<std::int32_t>();
	vec_psOutputs.reserve(size);

	for (std::int32_t i = 0; i < size; i++) {
		vec_psOutputs.push_back(inputLayoutReader.ReadVariable<DXGI_FORMAT>());
	}

	if (vec_psOutputs.size() == 0) {
		vec_psOutputs.push_back(DXGI_FORMAT_R8G8B8A8_UNORM);
	}

}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& fileName, Value_ptr<GraphicDevice> arg_graphicDevice)
{
	Dx12Compile::ReadCompiled(fileName,geometryShaderBlob);
	shaderName = StringHelper::GetFileName(fileName, false);
}

ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::~Resource_GeometryShader_Dx12()
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::Attach() const
{
}

void ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice)
{
	vwp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::GetGeometryShaderBlob()
{
	return geometryShaderBlob;
}

std::string ButiEngine::ButiRendering::Resource_GeometryShader_Dx12::GetName() const
{
	return shaderName;
}
